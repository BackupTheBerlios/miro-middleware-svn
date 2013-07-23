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
#ifndef PsosConnection_h
#define PsosConnection_h

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/TtyConnection.h"

namespace Psos
{
  // forward declarations
  class Message;
  class EventHandler;
  class Parameters;

  class Connection : public Miro::TtyConnection
  {
    typedef Miro::TtyConnection Super;

  public:
    Connection(ACE_Reactor* _reactor, 
	       EventHandler* _eventHandler,
	       const Parameters& _parameters);
    virtual ~Connection();

    void writeMessage(const Message& message);
    bool synched() const;
    bool waitSynch(const ACE_Time_Value& maxWait) const;
    void close();

  protected:
    EventHandler* eventHandler;
    int synchTimerId;
    int watchdogTimerId;

    Miro::Mutex writeMutex;
    ACE_Time_Value lastWrite;

    static const ACE_Time_Value writeTimeOut;
  };
}
#endif
