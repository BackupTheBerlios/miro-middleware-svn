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


#include "PanTiltConsumer.h"

#include <iostream>

#include <netinet/in.h>

// #undef DEBUG

namespace DpPanTilt
{
  using std::string;

  //////////////////////////////////////////////////////////////////////
  // Consumer
  //
  Consumer::Consumer(Data& panTiltData_) :
    Log(WARNING,"Consumer"),
    panTiltData(panTiltData_),
    packetsProcessed(0),
    packetsCorrupted(0)
  {
  }

  ////////////////////////////////////////////////////////////////////////
  // statistics
  void Consumer::incStatistics(bool corrupted) {

    if (corrupted) 
      ++packetsCorrupted;

    ++packetsProcessed;


    // show statistic rarely
    if (!(packetsProcessed % 5000)) {
      log(INFO,"Statistics:");
      cerr << "total : " << packetsProcessed << ", " 
	   << (100.0 * packetsCorrupted / packetsProcessed) 
	   << " % corrupted." << endl;
    }
  }

  ////////////////////////////////////////////////////////////////////////
  // handleMessage
  //
  void
  Consumer::handleMessage(const Miro::DevMessage * _message)
  {
    const Message& message = *static_cast<const Message *>(_message);
    
//     cerr << "handleMessage |" << message << "|" <<endl;

    panTiltData.sync.acquire();
    panTiltData.failed = false;

    if (message.getErrorState()) {
      log(ERROR, "received error message, last command probably failed, aborting it.");
      cerr << "|" << message.getMessage()  << "|"  << endl;
      incStatistics( true );
      
      panTiltData.failed = true;
      panTiltData.sync.release();

      panTiltData.syncCond.broadcast();
      return;
    }

    int opcode = FIRSTOPCODE;
    while (opcode<=LASTOPCODE) {
      if ( message.getMessage().length() >= panTiltMessages[opcode].length() )
	if ( string(message.getMessage(),0 , panTiltMessages[opcode].length()) == panTiltMessages[opcode]) {
	  break;
      }
	
      ++opcode;
    }

    if (message.getMessage().length() == 1) {
      // special case, just a * <delim> packet
      // this happens after A(wait) commands
      log(INFO,"got empty packet, interpreted as ack");
      panTiltData.sync.release();
      panTiltData.syncCond.broadcast();
      incStatistics( true );
      return;
    }

    if (opcode > LASTOPCODE) {
      log(ERROR,"ignored unknown or corrupted message :");
      cerr << "|" << message.getMessage()  << "|"  << endl;
      panTiltData.sync.release();
      incStatistics( false );

      return;
    }

//     cerr << "found match for |" << panTiltMessages[opcode] << "|" << endl;

    // be optimistic
    bool gotdata = true;

    // now do something appro.
    switch (opcode) {
    case PANPOSITION: {
      panTiltData.panPosition = message.getLong( panTiltMessages[opcode].length() );
    }
    break; 
    case TILTPOSITION: 
      panTiltData.tiltPosition = message.getLong( panTiltMessages[opcode].length() );
      break;
    case TARGETPANPOSITION: 
      panTiltData.targetPanPosition = message.getLong( panTiltMessages[opcode].length() );
      break;
    case TARGETTILTPOSITION: 
      panTiltData.targetTiltPosition = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case MINPANPOSITION: 
      panTiltData.minPanPosition = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case MAXPANPOSITION: 
      panTiltData.maxPanPosition = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case MINTILTPOSITION: 
      panTiltData.minTiltPosition = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case MAXTILTPOSITION: 
      panTiltData.maxTiltPosition = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case LIMITBOUNDSENABLED: 
      panTiltData.limitbounds = true;
      break; 
    case LIMITBOUNDSDISABLED: 
      panTiltData.limitbounds = false;
      break; 
    case TARGETPANSPEED: 
      panTiltData.targetPanSpeed = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case TARGETTILTSPEED: 
      panTiltData.targetTiltSpeed = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case PANACCELERATION: 
      panTiltData.panAcceleration = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case TILTACCELERATION: 
      panTiltData.tiltAcceleration = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case PANBASESPEED: 
      panTiltData.panBaseSpeed = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case TILTBASESPEED: 
      panTiltData.tiltBaseSpeed = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case MAXPANSPEED: 
      panTiltData.maxPanSpeed = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case MINPANSPEED: 
      panTiltData.minPanSpeed = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case MAXTILTSPEED: 
      panTiltData.maxTiltSpeed = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case MINTILTSPEED: 
      panTiltData.minTiltSpeed = message.getLong( panTiltMessages[opcode].length() );
      break; 
    case ECHOINGON:
      panTiltData.echoing = true;
      break; 
    case ECHOINGOFF: 
      panTiltData.echoing = false;
      break; 
    case ASCIIVERBOSE: 
      panTiltData.verbose = true;
      break; 
    case ASCIITERSE: 
      panTiltData.verbose = false;
      break; 
    case VERSION: 
      break; 
    case PANHOLDLOW: 
      panTiltData.panHold = 1;
      break; 
    case PANHOLDREGULAR: 
      panTiltData.panHold = 2;
      break; 
    case PANHOLDOFF: 
      panTiltData.panHold = 0;
      break; 
    case TILTHOLDLOW: 
      panTiltData.tiltHold = 1;
      break; 
    case TILTHOLDREGULAR: 
      panTiltData.tiltHold = 2;
      break; 
    case TILTHOLDOFF: 
      panTiltData.tiltHold = 0;
      break; 
    case PANMOVELOW: 
      panTiltData.panMove = 0;
      break; 
    case PANMOVEREGULAR: 
      panTiltData.panMove = 1;
      break; 
    case PANMOVEHIGH: 
      panTiltData.panMove = 2;
      break; 
    case TILTMOVELOW: 
      panTiltData.tiltMove = 0;
      break; 
    case TILTMOVEREGULAR: 
      panTiltData.tiltMove = 1;
      break; 
    case TILTMOVEHIGH: 
      panTiltData.tiltMove = 2;
      break;
    default:
      gotdata = false;
    }

    panTiltData.sync.release();

    if (gotdata) 
      panTiltData.syncCond.broadcast();

    incStatistics( true );
    return;
  }
};
