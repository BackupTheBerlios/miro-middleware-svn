// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "BaseImpl.h"
#include "BaseConnection.h"
#include "BaseConsumer.h"
#include "BaseDevice.h"
#include "BaseStatus.h"

#include "mcp/McpMessage.h"
#include "mcp/McpDevice.h"

// #include <cmath>
#include <iostream>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Base
{
  using std::cout;
  using std::cerr;

  /***************************************************************************
   *
   *  Base Consumer Module 
   *
   *  Handles output from base
   *
   ***************************************************************************/

  Consumer::Consumer(Connection& _connection,
		     Miro::OdometryImpl * _odometry) :
    Super(_connection),
    connection(_connection),
    status(_odometry),
    rotateHaltMutex(),
    rotateHaltCond(rotateHaltMutex),
    translateHaltMutex(),
    translateHaltCond(translateHaltMutex),
    rotateCurrentMutex(),
    rotateCurrentCond(rotateCurrentMutex),
    rotateCurrent(),
    translateCurrentMutex(),
    translateCurrentCond(translateCurrentMutex),
    translateCurrent(),
    batteryVoltageMutex(),
    batteryVoltageCond(batteryVoltageMutex),
    batteryVoltage()
  {
  }

  Consumer::~Consumer() {
    DBG(cout << "Destructing BaseConsumer" << endl); 

    rotateCurrentCond.broadcast();
    translateCurrentCond.broadcast();
    batteryVoltageCond.broadcast();
  }

  // handlers for asynchronous base events

  void
  Consumer::handleRotateCurrent(unsigned long value)
  {
    DBG(cout << "handleRotateCurrent: 0x" << hex << value << dec << endl);

    rotateCurrentMutex.acquire();
    rotateCurrent = value;
    rotateCurrentMutex.release();
    rotateCurrentCond.broadcast();
  }

  void
  Consumer::handleRotateHalt() 
  {
    DBG(cout << "handleRotateHalt()" << endl);

    //    Miro::Guard guard(connection.stateMutex);
    //    connection.rotateState = LIMP;
    rotateHaltCond.broadcast();
  }

  void
  Consumer::handleTranslateCurrent(unsigned long value)
  {
    DBG(cout << "handleTranslateCurrent: 0x" << hex << value << dec << endl);

    translateCurrentMutex.acquire();
    translateCurrent = value;
    translateCurrentMutex.release();
    translateCurrentCond.broadcast();
  }

  void
  Consumer::handleTranslateHalt() 
  {
    DBG(cout << "handleTranslateHalt()" << endl);

    //    Miro::Guard guard(connection.stateMutex);
    //    connection.translateState = LIMP;
    translateHaltCond.broadcast();
  }

  void
  Consumer::handleBatteryVoltage(unsigned long value)
  {
    DBG(cout << "handleBatteryVoltage: 0x" << hex << value << dec << endl);

    batteryVoltageMutex.acquire();
    batteryVoltage = (double) value / BATTERY_UNITS_PER_VOLT;
    batteryVoltageMutex.release();
    batteryVoltageCond.broadcast();
  }

  //
  // protected methods
  //

  // initial handling of the base event by parsing
  // the message string

  void
  Consumer::handleError(const Mcp::Message& message) 
  {
    unsigned char errorByte = message.value();

    connection.errorAcknowledge(0); // ack all errors for now

    cerr << "Received error. Type=";

    if (errorByte & TERR) {
      cerr << "TERR" << endl;
      handleTranslateError();
    }
    else if (errorByte & TC) {
      cerr << "TC" << endl;
      handleTranslateError();
    }
    else if (errorByte & RERR) {
      cerr << "RERR" << endl;
      handleRotateError();
    }
    else if (errorByte & RC) {
      cerr << "RC" << endl;
      handleRotateError();
    }
    else if (errorByte & BHI) {
      cerr << "BHI" << endl;
      handleBatteryHigh();
    }
    else if (errorByte & BLO) {
      cerr << "BLO" << endl;
      handleBatteryLow();
    }
    else {
      cerr << "0x" << (int)errorByte << endl;
    }
  }

  void
  Consumer::handleMessage(const Miro::DevMessage* message)
  {
    const Mcp::Message& msg = (const Mcp::Message&)*message;

    DBG(cout << "base: handleMessage - OpCode: 0x" << hex << (int)msg.opcode() << dec << endl);

    switch(msg.opcode()) {
    case OP_ROT_CURRENT:	
      handleRotateCurrent(msg.value()); 
      break;

    case OP_ROT_WHERE:	
      handleRotateWhere(msg.value());
      break;
      
    case OP_BATTERY_VOLTAGE:
      handleBatteryVoltage(msg.value());
      break;

    case OP_BATTERY_CURRENT: 
      handleBatteryCurrent(msg.value());
      break;
      
    case OP_TRANS_CURRENT:
      handleTranslateCurrent(msg.value());
      break;
    
    case OP_TRANS_WHERE:
      handleTranslateWhere(msg.value());
      break;

    case OP_INDEX_REPORT:
      break;
    
    default:
      Super::handleMessage(message);
    }
  }

  void 
  Consumer::handleStatusReport(const Mcp::Message& message)
  {
    status <<= message;  // set base status data according to status report
  
  
    if (status.rotateHalt()) { // base just stopped rotating
      DBG(cout << "handleStatusReport() - rotate halt" << endl);
  
      handleRotateHalt();
    } else if (status.passiveRotation()) {
      DBG(cout << "handleStatusReport() - ROTATE HALT?" << endl);
      connection.sendCmd(&MSG_ROT_HALT);
    }

    if (status.translateHalt()) { // base just stopped moving
      DBG(cout << "handleStatusReport() - translate halt" << endl);
      handleTranslateHalt();
    } else if (status.passiveTranslation()) {
      DBG(cout << "handleStatusReport() - TRANSLATE HALT?" << endl);
      connection.sendCmd(&MSG_TRANS_HALT);
    }
  }
}
