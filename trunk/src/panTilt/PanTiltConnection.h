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
#ifndef PanTiltConnection_h
#define PanTiltConnection_h

#include "miro/Exception.h"
#include "miro/TtyConnection.h"

namespace DpPanTilt
{
  // forward declarations
  class Message;
  class EventHandler;
  class Parameters;
  class Data;

  class Connection : public Miro::TtyConnection
  {
    typedef Miro::TtyConnection Super;

  public:
    Connection(ACE_Reactor * _reactor, 
	       EventHandler * _eventHandler,
	       Data& _data);
    virtual ~Connection();

    /**
     * tries to send pakets to the panTilt, (status request)
     * if the paket is acknowledged it returns true,
     * does five retries at the moment
     */
    void checkConnection();
    void writeMessage(Message& message);

  protected:
    EventHandler * eventHandler;
    long startupTimerId;
    Data& data_;
  };
};
#endif
