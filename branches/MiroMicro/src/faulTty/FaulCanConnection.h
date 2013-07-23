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
#ifndef FaulCanConnection_h
#define FaulCanConnection_h

#include "FaulControllerConnection.h"
#include "sparrow/SparrowConnection2003.h"

#include "miro/Synch.h"

// forward declarations
namespace FaulMotor {
  class Connection;
}

namespace FaulController
{
  class FaulCanConnection : public Connection
  {
    //! Super class accessor.
    typedef Connection Super;

  public:
    FaulCanConnection(Sparrow::Connection2003 * _connection2003, int _motor);
    virtual ~FaulCanConnection();

    virtual void sendAccVelTicks(short accL, short accR, 
				 short velL, short velR);
    virtual void sendAccVelTicks(short acc, short vel);

    virtual void writeBinary(char const * buffer, int _len);
    virtual void writeMessage(char const * const _message[]);

  protected:
    Sparrow::Connection2003 * connection2003_;
    int motor_;

    Miro::Mutex mutex_;
    ACE_Time_Value lastWrite_;
  };
}
#endif
