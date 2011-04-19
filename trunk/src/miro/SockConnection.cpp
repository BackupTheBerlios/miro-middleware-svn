// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005 Defence Research and Development Canada, Suffield
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
// Authors: G. Broten
//

/* Standard Includes */
#include <iostream>

/* ACE Includes */
#include <ace/Reactor.h>
#include <ace/OS.h>

/* Miro Includes */
#include <miro/DevEventHandler.h>
#include <miro/Exception.h>
#include <miro/Log.h>

/* Local Includes */
#include "SvcParameters.h"
#include "SockConnection.h"

namespace Miro
{

  //
  // Constructors / Destructors
  //

  SockConnection::SockConnection(ACE_Reactor * _reactor,
                                 DevEventHandler * _eventHandler,
                                 const SockParameters& _parameters) :
      reactor_(_reactor),
      eventHandler_(_eventHandler),
      hostName_(_parameters.host.c_str()),
      portName_(_parameters.port.c_str())
  {

    MIRO_DBG(MIRO, LL_NOTICE, "SockConnection initialising\n");

    /*
    **  Initialize and pointer to a socket as the connection method
    */

    if (srvr_.set(portName_, hostName_) == -1) {
      MIRO_LOG_OSTR(LL_CRITICAL,
                    "Failed to set Address and Port: " << portName_ << hostName_);
      throw CException(errno, strerror(errno));
    }

    if (connector_.connect(peer_, srvr_, 0, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      MIRO_LOG_OSTR(LL_CRITICAL,
                    "Failed to open device: " << hostName_ << ":" << portName_
                    << "\nEnsure the required Server is running?");
      throw CException(errno, strerror(errno));
    }

    // we need a handler to listen to the asynchronous file handler
    // of the device
    eventHandler_->set_handle(peer_.get_handle());
    int rc =
      reactor_->register_handler(eventHandler_, ::ACE_Event_Handler::READ_MASK);
    if (rc == -1)
      throw ACE_Exception(errno, "Failed to register handler for file descriptor.");
    MIRO_DBG(MIRO, LL_NOTICE, "SockConnection Complete\n");

  }

  SockConnection::~SockConnection()
  {
    MIRO_DBG(MIRO, LL_NOTICE, "Destructing SockConnection\n");

    // Stop hardware triggered communication
    reactor_->remove_handler(eventHandler_,
                             ::ACE_Event_Handler::READ_MASK);
    peer_.close();
  }

}
