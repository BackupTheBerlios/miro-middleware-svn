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
