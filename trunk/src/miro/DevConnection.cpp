// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
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

#include <ace/Reactor.h>
#include <miro/Log.h>

namespace Miro
{
  //
  // Constructors / Destructors
  //

  DevConnection::DevConnection(ACE_Reactor * _reactor,
			       DevEventHandler * _eventHandler,
			       const DevParameters& _parameters) :
    reactor(_reactor),
    eventHandler(_eventHandler),
    devName(_parameters.device.c_str()),
    ioBuffer(),
    connector(),
    selectHandlerId(-1),
    tv(0,0)
  {
    MIRO_DBG(MIRO,LL_NOTICE,"DevConnection intizialising\n");

    if (connector.connect(ioBuffer, devName, &tv, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      MIRO_LOG(LL_CRITICAL, "Failed to open device.\nPropably running on the wrong machine?\n");
      throw CException(errno, std::strerror(errno));
    }

    // we need a handler to listen to the asynchronous file handler
    // of the device
    selectHandlerId =
      reactor->register_handler(ioBuffer.get_handle(),
				eventHandler, ::ACE_Event_Handler::READ_MASK);
    if (selectHandlerId == -1)
      throw ACE_Exception(errno, "Failed to register handler for file descriptor.");
  }

  DevConnection::~DevConnection()
  {
    MIRO_DBG(MIRO,LL_NOTICE, "Destructing DevConnection\n");

    // Stop hardware triggered communication
    if (selectHandlerId != -1)
      reactor->remove_handler(selectHandlerId);
    ioBuffer.close();
  }
};
