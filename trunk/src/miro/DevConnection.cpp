// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999, 2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "DevConnection.h"
#include "DevEventHandler.h"
#include "Parameters.h"
#include "Exception.h"
#include "Log.h"

#include <ace/Reactor.h>

namespace Miro
{
  //
  // Constructors / Destructors
  //

  DevConnection::DevConnection(ACE_Reactor * _reactor,
			       DevEventHandler * _eventHandler,
			       const DevParameters& _parameters) :
    reactor_(_reactor),
    eventHandler_(_eventHandler),
    devName_(_parameters.device.c_str()),
    ioBuffer_(),
    connector_()
  {
    MIRO_DBG(MIRO, LL_NOTICE, "DevConnection intizialising\n");
    
    ACE_Time_Value tv(0,0); // none blocking open
    if (connector_.connect(ioBuffer_, 
			   devName_, 
			   &tv, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      MIRO_LOG_OSTR(LL_CRITICAL,
		    "Failed to open device: " << _parameters.device <<
		    "\nPropably running on the wrong machine?\n");
      throw CException(errno, std::strerror(errno));
    }

    // we need a handler to listen to the asynchronous file handler
    // of the device
    eventHandler_->set_handle(ioBuffer_.get_handle());
    int rc = reactor_->register_handler(eventHandler_, ::ACE_Event_Handler::READ_MASK);
    if (rc == -1)
      throw ACE_Exception(errno, "Failed to register handler for file descriptor.");
  }

  DevConnection::~DevConnection()
  {
    MIRO_DBG(MIRO, LL_NOTICE, "Destructing DevConnection\n");

    // Stop hardware triggered communication
    reactor_->remove_handler(eventHandler_, ::ACE_Event_Handler::READ_MASK);
    ioBuffer_.close();
  }
}
