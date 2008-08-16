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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#include "DevConnection.h"
#include "DevEventHandler.h"
#include "SvcParameters.h"
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
      throw CException(errno, strerror(errno));
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
