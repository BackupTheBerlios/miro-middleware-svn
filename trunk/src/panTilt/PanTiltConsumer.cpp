// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "PanTiltConsumer.h"
#include "miro/Log.h"

#include <string>
#include <iostream>

namespace DpPanTilt
{
  //////////////////////////////////////////////////////////////////////
  // Consumer
  //
  Consumer::Consumer(Data& panTiltData_) :
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
      MIRO_LOG_OSTR(LL_NOTICE, "Statistics - total: "<< packetsProcessed << ", " << (100.0 * packetsCorrupted / packetsProcessed) <<"% corrupted.\n" );
    }
  }

  ////////////////////////////////////////////////////////////////////////
  // handleMessage
  //
  void
  Consumer::handleMessage(const Miro::DevMessage * _message)
  {
    const Message& message = *static_cast<const Message *>(_message);
    
    panTiltData.sync.acquire();
    panTiltData.failed = false;

    if (message.getErrorState()) {
      MIRO_LOG(LL_ERROR, "Received error message, last command probably failed, aborting it.");
      MIRO_LOG_OSTR(LL_NOTICE, message.getMessage().c_str());
      incStatistics( true );
      
      panTiltData.failed = true;
      panTiltData.sync.release();

      panTiltData.syncCond.broadcast();
      return;
    }

    int opcode = FIRSTOPCODE;
    while (opcode<=LASTOPCODE) {
      if (message.getMessage().length() >= panTiltMessages[opcode].length()  &&
	  std::string(message.getMessage(), 0, panTiltMessages[opcode].length()) == panTiltMessages[opcode]) {
	break;
      }
	
      ++opcode;
    }

    if (message.getMessage().length() == 1) {
      // special case, just a * <delim> packet
      // this happens after A(wait) commands
      MIRO_LOG(LL_NOTICE, "Got empty packet, interpreted as ack.\n");
      panTiltData.sync.release();
      panTiltData.syncCond.broadcast();
      incStatistics( true );
      return;
    }

    if (opcode > LASTOPCODE) {
      MIRO_LOG_OSTR(LL_WARNING, 
		   "Ignored unknown or corrupted message : " << message.getMessage().c_str() << "\n");
      panTiltData.sync.release();
      incStatistics( false );

      return;
    }

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
    case PANVERSION: 
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
}
