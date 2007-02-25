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
#ifndef AbusConnection_h
#define AbusConnection_h

#include "miro/DevConnection.h"
#include "miro/Exception.h"

// forward declerations
namespace Miro
{
  class Exception;
};

namespace Abus
{
  // forward declerations
  class Message;

  class Connection : Miro::DevConnection
  {
    typedef Miro::DevConnection Super;

  public:
    Connection(ACE_Reactor * _reactor, 
	       Miro::DevEventHandler * _eventHandler,
	       const Miro::DevParameters& _parameters);
    virtual ~Connection();
  
    // possible commands to send to all mcp devices

    void reset       () throw(Miro::Exception);
    void linkRequest (const char *prot, const char *type, const char *model) throw(Miro::Exception);

    void linkApprove (unsigned char devId, unsigned char approve) throw(Miro::Exception);
    void getLongId   (unsigned char devId) throw(Miro::Exception);
    void getId       (unsigned char devId) throw(Miro::Exception);
    void getType     (unsigned char devId) throw(Miro::Exception);
    void getStatus   (unsigned char devId) throw(Miro::Exception);
    void enable      (unsigned char devId, unsigned char enable) throw(Miro::Exception);
    void disconnect  (unsigned char devId) throw(Miro::Exception);

    //  protected:
    void sendMessage (const Abus::Message* msp) throw(Miro::Exception);
  };
};

#endif




