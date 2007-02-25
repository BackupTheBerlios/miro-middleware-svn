// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
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
//   Ingo van Lil
//
#include <cmath>
#include <ace/Reactor.h>
#include <ace/OS.h>

#include "GpsConnection.h"
#include "GpsEventHandler.h"
#include "Parameters.h"

#include "miro/Exception.h"
#include "miro/Log.h"

namespace GPS
{
  Connection::Connection(ACE_Reactor * _reactor,
	  EventHandler * _eventHandler,
	  const Parameters& _parameters) :
    Super(_reactor, _eventHandler, _parameters),
    eventHandler(_eventHandler)
  {
    MIRO_LOG_CTOR("GPS::Connection");
    MIRO_LOG_CTOR_END("GPS::Connection");
  }

  Connection::~Connection()
  {
    MIRO_LOG_DTOR("GPS::Connection");
    MIRO_LOG_DTOR_END("GPS::Connection");
  }

  void Connection::send(const std::string command)
  {
    MIRO_LOG_OSTR(LL_PRATTLE, "Sending GPS command: " << command);
    std::string cmd = command + "\r\n";
    ioBuffer.send_n(cmd.c_str(), cmd.length());
  }
}

