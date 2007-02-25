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
#include "MspEventHandler.h"
#include "MspConnection.h"
#include "MspMessage.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include "abus/AbusDevice.h"


namespace Msp
{
  EventHandler::EventHandler(Connection& _msp, 
			     Miro::DevConsumer * _consumer) :
    Super(_msp, _consumer, new Message()),
    msp(_msp)
  {
    MIRO_LOG_CTOR("Msp::EventHandler");
  }

  EventHandler::~EventHandler() 
  {
    MIRO_LOG_DTOR("Msp::EventHandler");
  }

  //-----------------------------------------------------------------------------
  // protected
  //-----------------------------------------------------------------------------
  void
  EventHandler::disconnect(const Abus::Message& message)
  {
    Super::disconnect(message);

    int mspAddr = Abus::devId2devNum(message.devId());

    MIRO_LOG_OSTR(LL_NOTICE, 
		  "msp.disconnect() Addr: 0x" << std::hex << mspAddr << std::dec);

    if (mspAddr == masterMspAddr) { // master MSP down. 
      msp.masterMspBusId = 0;
      msp.sonarsPinging = false;    // sonars have stopped pinging
    }
  }
    
  void
  EventHandler::linkApproveAck(const Abus::Message& message)
  {
    Super::linkApproveAck(message);

    int mspAddr = Abus::devId2devNum(message.devId());

    if (mspAddr == masterMspAddr)       // master msp up 
      msp.masterMspBusId = message.devId();

    MIRO_LOG_OSTR(LL_NOTICE,
		  "msp.linkApproveAck() Addr: 0x" << std::hex << mspAddr << std::dec);

    try {
      msp.setIrParms(message.devId(), 75); // set ir interval

      if (msp.sonarsPinging) {
	msp.sonarStop();            // stop-start to include new MSP
	msp.sonarStart();
      } 
      else if (mspAddr == masterMspAddr && msp.sonarsRequested) { // master msp up 
	msp.sonarStart();           // start pinging
      }
    } catch (const Miro::CException& e) {
      MIRO_LOG_OSTR(LL_ERROR, "msp.lingApproveAck() exception: " << e);
    } catch (const Miro::Exception& e) {
      MIRO_LOG_OSTR(LL_ERROR, "msp.lingApproveAck() exception: " << e);
    }
  }
}
