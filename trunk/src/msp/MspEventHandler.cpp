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

#include "abus/AbusDevice.h"

#undef DEBUG

namespace Msp
{
  using std::cout;
  using std::cerr;
  using std::endl;

  EventHandler::EventHandler(Connection& _msp, 
			     Miro::DevConsumer * _consumer) :
    Super(_msp, _consumer, new Message()),
    msp(_msp)
  {
#ifdef DEBUG
    cout << "Constructing MspEventHandler." << endl;
#endif
  }

  EventHandler::~EventHandler() 
  {
#ifdef DEBUG
    cout << "Destructing MspEventHandler." << endl;
#endif
  }

  //-----------------------------------------------------------------------------
  // protected
  //-----------------------------------------------------------------------------
  void
  EventHandler::disconnect(const Abus::Message& message)
  {
    Super::disconnect(message);

    int mspAddr = Abus::devId2devNum(message.devId());

#ifdef DEBUG
    cout << "msp.disconnect() Addr: 0x" << hex << mspAddr << endl;
#endif

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

#ifdef DEBUG
    cout << "msp.linkApproveAck() Addr: 0x" << hex << mspAddr << endl;
#endif

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
      cerr << "msp.lingApproveAck() exception: " << e << endl;
    } catch (const Miro::Exception& e) {
      cerr << "msp.lingApproveAck() exception: " << e << endl;
    }
  }
};
