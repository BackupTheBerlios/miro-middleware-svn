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

#include <ace/Reactor.h>

#include <iostream>

#include "abus/AbusDevice.h"

#include "MspConnection.h"
#include "MspDevice.h"
#include "MspMessage.h"
#include "Parameters.h"

// #undef DEBUG
// #undef VERBOSE

#ifdef DEBUG
#include <iostream>
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Msp
{
  using Miro::DevEventHandler;
  using std::cout;
  using std::cerr;
  using std::endl;


  const unsigned long Connection::ping1[7] = 
  {
    0x2001, 0x2011, 0x2101, 0x2111, 0x2201, 0x2211, 0x0000
  };
  const unsigned long Connection::ping2[7] = 
  {
    0x2002, 0x2012, 0x2102, 0x2112, 0x2202, 0x2212, 0x0000
  };
  const unsigned long Connection::ping3[7] = 
  {
    0x2003, 0x2013, 0x2103, 0x2113, 0x2203, 0x2213, 0x0000
  };
  const unsigned long Connection::ping4[7] = 
  {
    0x2004, 0x2014, 0x2104, 0x2114, 0x2204, 0x2214, 0x0000
  };

  const unsigned long * Connection::globalSonarTable[5] = 
  {
    ping1, ping2, ping3, ping4, NULL // NULL to end table
  };

  //
  // Constructors / Destructors
  //

  Connection::Connection(ACE_Reactor* _reactor, 
			 Miro::DevEventHandler * _eventHandler) :
    Super(_reactor, _eventHandler, *Parameters::instance()),
    sonarsPinging(false),
    sonarsRequested(false),
    masterMspBusId(0)
  {
    DBG(cout << "Constructing MspConnection." << endl);

    // readIrCalibrationData();
    // readTactileMask();
    // readSonarMask();
  }

  Connection::~Connection()
  {
    DBG(cout << "Destructing Msp" << endl);
  }


  //--------------------------------------------------------------------------
  // functions that specify server behaviour
  //--------------------------------------------------------------------------
  void 
  Connection::sonarStart() throw(Miro::Exception)
  {
    sonarsRequested = true;

    DBG(cerr << "sonarStart()" << endl);

    if (masterMspBusId != 0 && 
	Abus::deviceTable[masterMspBusId].state == Abus::READY) {
      setSonarTable(masterMspBusId, globalSonarTable);
      reqSonarStart(masterMspBusId);
      sonarsPinging = true;
    }
    else {
      sonarsPinging = false;
    }
  }

  void
  Connection::sonarStop() throw(Miro::Exception)
  {
    if (masterMspBusId != 0 && 
	Abus::deviceTable[masterMspBusId].state == Abus::READY) {
      reqSonarStop(masterMspBusId);
    }
    sonarsRequested = false;
    sonarsPinging = false;
  }

  //--------------------------------------------------------------------------
  // functions to send messages to an MSP
  //--------------------------------------------------------------------------

  void
  Connection::reqReset(unsigned char devId) throw(Miro::Exception)
  {
    Abus::Message msg(Abus::D2M_MSG_TO_DEVICE, 1, devId);

    msg.message()[0] = Abus::AB_RESET;
    msg.msgLen() += 1;

    sendMessage(&msg);
  }

  void 
  Connection::reqVer(unsigned char devId) throw(Miro::Exception) 
  {
    Message msg(devId, AB_RWI_VR_REQ);

    sendMessage(&msg);
  }

  void
  Connection::reqIrParms(unsigned char devId) throw(Miro::Exception) 
  {
    Message msg(devId, MSP_IR_PARMS);

    sendMessage(&msg);
  }

  void
  Connection::setIrParms(unsigned char devId, unsigned short interval) throw(Miro::Exception) 
  {
    Message msg(devId, MSP_IR_PARMS);

    msg.setShortData(0, IR_PARM_INTERVAL);
    msg.setShortData(1, interval);

    msg.msgLen() += 2 * sizeof(short);

    sendMessage(&msg);
  }

  void
  Connection::reqSonarTable (unsigned char devId) throw(Miro::Exception)  
  {
    Message msg(devId,  MSP_SON_TABLE);

    sendMessage(&msg);
  }

  void
  Connection::setSonarTable (unsigned char devId, const unsigned long *table[]) throw(Miro::Exception)
  {
    Message msg(devId, MSP_SON_TABLE);

    int count, set, xducer;
  
    count = 0;
    set = 0;
    while (table[set]) {
      xducer = 0;
      while (table[set][xducer]) {
	msg.setShortData(count++, table[set][xducer++]);
      }
      msg.setShortData(count++, 0);
      ++set;
    }
    msg.msgLen() += (count-1) * sizeof(short);

    sendMessage(&msg);
  }

  void
  Connection::reqSonar(unsigned char devId, const unsigned long *table) throw (Miro::Exception) 
  {
    Message msg(devId, MSP_SON_REQ);

    int count = 0;
  
    do {
      msg.setShortData(count, table[count]);
    }
    while (table[count++]);

    msg.msgLen() += (count - 1) * sizeof(short);

    sendMessage(&msg);
  }

  void
  Connection::reqSonarStart(unsigned char devId) throw (Miro::Exception) 
  {
    Message msg(devId, MSP_SON_START);

    msg.setCharData(0, SONAR_START);
    msg.msgLen() += 1;

    sendMessage(&msg);
  }

  void
  Connection::reqSonarStop(unsigned char devId) throw (Miro::Exception) 
  {
    Message msg(devId, MSP_SON_START);

    msg.setCharData(0, SONAR_STOP);
    msg.msgLen() += 1;

    sendMessage(&msg);
  }

  void
  Connection::reqSonarParms(unsigned char devId) throw (Miro::Exception) 
  {
    Message msg(devId, MSP_SON_PARMS);

    sendMessage(&msg);
  }

  void
  Connection::setSonarParms(unsigned char devId, const SonParmsType *parms) throw (Miro::Exception)
  {
    Message msg(devId, MSP_SON_PARMS);

    int count = 0;
  
    if (parms->echoCount >= 0) {
      msg.setShortData(count++, SONAR_ECHO_NUMBER);
      msg.setShortData(count++, parms->echoCount);
    }

    if (parms->echoTimeout >= 0) {
      msg.setShortData(count++, SONAR_ECHO_TIMEOUT);
      msg.setShortData(count++, parms->echoTimeout);
    }

    if (parms->fireDelay >= 0) {
      msg.setShortData(count++, SONAR_FIRE_DELAY);
      msg.setShortData(count++, parms->fireDelay);
    }

    if (parms->fireInterval >= 0) {
      msg.setShortData(count++, SONAR_FIRE_INTERVAL);
      msg.setShortData(count++, parms->fireInterval);
    }

    if (parms->echoBlankTime >= 0) {
      msg.setShortData(count++, SONAR_ECHO_BLNK_TIME);
      msg.setShortData(count++, parms->echoBlankTime);
    }

    if (parms->initialBlankTime >= 0) {
      msg.setShortData(count++, SONAR_INIT_BLNK_TIME);
      msg.setShortData(count++, parms->initialBlankTime);
    }

    if (parms->startDelay >= 0) {
      msg.setShortData(count++, SONAR_START_DELAY);
      msg.setShortData(count++, parms->startDelay);
    }

    msg.msgLen() += count * sizeof(short);

    sendMessage(&msg);
  }
};
