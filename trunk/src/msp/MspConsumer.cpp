// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "MspConsumer.h"
#include "MspMessage.h"
#include "MspDevice.h"
#include "MspConnection.h"
#include "Parameters.h"

#include "abus/AbusDevice.h"

#include "base/BaseStatus.h"

#include "idl/RangeEventC.h"
#include "miro/RangeSensorImpl.h"
#include "miro/StructuredPushSupplier.h"

#include <iostream>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cin;
using std::cerr;

using Miro::RangeBunchEventIDL;

namespace Msp
{
  // sonaddr -> sonar number
  std::map<unsigned long, int> Consumer::sonarAddr2Num;
  std::map<unsigned long, int> Consumer::mspAddr2Num;

  // stuff for normalizing the IR data

  bool normalizeIr = true;
  /* This is kinda ugly - we should really allocate these dynamically */
  // static int irMin[B_MAX_SENSOR_ROWS][B_MAX_SENSOR_COLS];
  // static int irMax[B_MAX_SENSOR_ROWS][B_MAX_SENSOR_COLS];

  // static char bmpMask[B_MAX_SENSOR_ROWS][B_MAX_SENSOR_COLS];

  Consumer::Consumer(Connection& _msp, 
	     Miro::RangeSensorImpl * _pSonar,
	     Miro::RangeSensorImpl * _pInfrared,
	     Miro::RangeSensorImpl * _pTactile) :
    Super(),
    msp(_msp),
    pSonar_(_pSonar),
    pInfrared_(_pInfrared),
    pTactile_(_pTactile)
  {
    DBG(cout << "Constructing MspConsumer" << endl);

    for (unsigned int i = 0; i < TACTILE_ROWS; ++i)
      for (unsigned int j = 0; j < TACTILE_COLS; ++j)
	tactileScan[i][j] = false;


    if (sonarAddr2Num.empty()) { // since its static we only initilize it once
      for (int i = 0; sonarAddr[i]; ++i)
	sonarAddr2Num[sonarAddr[i]] = i;
    }
    if (mspAddr2Num.empty()) { // since its static we only initilize it once
      for (int i = 0; mspAddr[i]; ++i)
	mspAddr2Num[mspAddr[i]] = i;
    }
  }

  Consumer::~Consumer() 
  {
    DBG(cout << "Destructing MspConsumer" << endl); 

    // unlock waiting threads
  }

  // handlers for asynchronous base events

  void
  Consumer::handleMessage(const Miro::DevMessage* msg)
  {
    const Message& message = (const Message&) *msg;

    DBG(cout << "mspMajorOp: 0x" << hex << (int)message.mspMajorOp() 
	<< " mspMinorOp: 0x" << (int)message.mspMinorOp() << dec << endl);

    if (message.minorOp() == 0) {  // protocol bit set 
      return;		            // not handled
    }
  
    switch (message.mspMajorOp()) {
    case AB_MSP_OPCODE:
      switch (message.mspMinorOp()) {
      case MSP_BMP_RPL:
	handleTactileReply(message);
	break;
      case MSP_IR_RPL:
	handleIrReply(message);
	break;
      case MSP_IR_PARMS_RPL:
	handleIrParmsReply(message);
	break;
      case MSP_SON_RPL:
	handleSonarReply(message);
	break;
      case MSP_SON_PARMS_RPL:
	handleSonarParmsReply(message);
	break;
      case MSP_SON_TABLE_RPL:
	handleSonarTableReply(message);
	break;
      default:
	Super::handleMessage(msg);
      }
      break;
    case AB_RWI_OPCODE:
      switch (message.mspMinorOp()) {
      case AB_RWI_VR_RPL:
	handleVersionReply(message);
	break;
      case AB_RWI_DBG_STR:
	handleDebugString(message);
	break;
      case AB_RWI_DBG_BIN:
	handleDebugBinary(message);
	break;
      default:
	Super::handleMessage(msg);
      }
      break;
    default:
      Super::handleMessage(msg);
      break;
    }
  }

  // RWI messages
  void 
  Consumer::handleVersionReply(const Message& DBG(msg))
  {
#ifdef DEBUG
    long mspNum = Abus::devId2devNum(msg.devId());

    cerr << "mspNum: 0x" << hex << mspNum << dec;

    if (msg.msgLen() > sizeof(2)) {
      // Verify NULL termination
      const_cast<Message&>(msg).message()[msg.msgLen()] = 0;

      cerr << " RWI version: " << msg.message() << endl;
    }
    else {
      cerr << " bad version" << endl;
    }
#endif
  }

  void 
  Consumer::handleDebugString(const Message& DBG(msg))
  {
#ifdef DEBUG
    long mspNum = Abus::devId2devNum(msg.devId());

    cerr << "mspNum: 0x" << hex << mspNum << dec;

    if (msg.msgLen() > sizeof(2)) {
      // Verify NULL termination
      const_cast<Message&>(msg).message()[msg.msgLen()] = 0;

      cerr << " RWI debug string: " << msg.message() << endl;
    }
    else {
      cerr << " bad RWI debug String" << endl;
    }
#endif
  }

  void
  Consumer::handleDebugBinary(const Message& DBG(msg))
  {
#ifdef DEBUG
    long mspNum = Abus::devId2devNum(msg.devId());  

    cerr << "mspNum: 0x" << hex << mspNum << endl;
    cerr << "0x" << hex;

    const unsigned char * dat = msg.mspData();
    int len = msg.msgLen() - sizeof(2);
  
    while (len--) {
      cerr << " " << (int)*dat++;
    }
    cerr << dec << endl;
#endif
  }

  void 
  Consumer::handleTactileReply(const Message& msg)
  {
    char switches = * msg.mspData();

    int mspAddr = Abus::devId2devNum(msg.devId());
    int tactileNum = mspAddr2Num[mspAddr];
    int row, col, length = 0;
    bool value;

    DBG(cerr << "mspConsumer: tactile reply" << endl);

    RangeBunchEventIDL * pTactileEvent = new RangeBunchEventIDL();
    pTactileEvent->time.sec  = msg.time().sec();
    pTactileEvent->time.usec = msg.time().usec();
    pTactileEvent->sensor.length(TACTILE_PER_MSP); // max length

    // iterate through new data
    for (int i = TACTILE_PER_MSP - 1; i >= 0; --i) {
      value = (switches & (0x01 << i));
      row = tactileNum2Row[tactileNum][i];
      col = tactileNum2Col[tactileNum][i];

      if (tactileScan[row][col] != value) {
	pTactileEvent->sensor[length].group = row;
	pTactileEvent->sensor[length].index = col;
	pTactileEvent->sensor[length].range = (!value)? 
	  Miro::RangeSensor::HORIZON_RANGE 
	  : 
	  0;
	++length;
      }
      tactileScan[row][col] = value;
    }
    pTactileEvent->sensor.length(length); // actual length
    if (length > 0) {
      pTactile_->integrateData(pTactileEvent);
    }
    else 
      delete pTactileEvent;
  }
 
  void 
  Consumer::handleIrReply(const Message& msg)
  {
    int mspAddr = Abus::devId2devNum(msg.devId());
    int irNum = mspAddr2Num[mspAddr];
    int row, col;
    CORBA::UShort value;

    RangeBunchEventIDL * pInfraredEvent = new RangeBunchEventIDL();
    pInfraredEvent->time.sec  = msg.time().sec();
    pInfraredEvent->time.usec = msg.time().usec();
    pInfraredEvent->sensor.length(IR_PER_MSP);

    DBG(cerr << "mspConsumer: ir reply" << endl);

    /* iterate through new data */

    for (int i = IR_PER_MSP - 1; i >= 0; --i) {
      row = irNum2Row[irNum][i];
      col = irNum2Col[irNum][i];

      pInfraredEvent->sensor[i].group = row;
      pInfraredEvent->sensor[i].index = col;

      value = msg.charData(i);

#ifdef NORMALIZE_IR_DEFINED
      if (normalizeIr) {
	if ((irMax[row][col] == 0) || 
	    (irMax[row][col] - irMin[row][col] < 0x60)) {
	  value = 0;
	}
	else {
	  value = (value-irMin[row][col]) * 99;
	  value /= irMax[row][col] - irMin[row][col];
	  if (value < 0) 
	    value = 0;
	}
      }
#endif
      pInfraredEvent->sensor[i].range = value;
    }

    pInfrared_->integrateData(pInfraredEvent);
  }

  void 
  Consumer::handleSonarReply(const Message& msg)
  {
    DBG(cerr << "mspConsumer: sonar reply" << endl);
      const SonarRplMessage& message = static_cast<const SonarRplMessage&>(msg);
    if (pSonar_ && message.sonars()) {

      int sonarNum;
      CORBA::UShort value;


      RangeBunchEventIDL * pSonarEvent = new RangeBunchEventIDL();
      pSonarEvent->time.sec  = msg.time().sec();
      pSonarEvent->time.usec = msg.time().usec();
      pSonarEvent->sensor.length(message.sonars());

      // iterate through new data

      for (int i = message.sonars() - 1; i >= 0; --i) {

	sonarNum = sonarAddr2Num[message.sonarId(i)];
	pSonarEvent->sensor[i].group = 0;
	pSonarEvent->sensor[i].index = sonarNum;

	value = message.sonarEcho(i, 0); // skip multiple echos for now

	value = (value < NO_RETURN)?
	  // This assumes that we're reporting sonar values in mm and
	  // that base_radius is in cm.  Otherwise, we will have to fudge  
	  // things differently.                                           
	  // (int) rint(((value * MM_PER_CLICK) + parameters.radius * 10.0) - 30.0) 
	  (int) rint(value * MM_PER_CLICK - 30.0) 
	  :
	  NO_RETURN - 1; // Miro::RangeSensor::HORIZON_RANGE ?!!

	pSonarEvent->sensor[i].range = value;
      }
      pSonar_->integrateData(pSonarEvent);
    }
  }
};
