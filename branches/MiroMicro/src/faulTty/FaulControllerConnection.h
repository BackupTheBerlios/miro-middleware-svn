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
#ifndef FaulControllerConnection_h
#define FaulControllerConnection_h

namespace FaulController
{
  // forward declarations
  class Connection
  {
  public:
    Connection();
    virtual ~Connection();

    virtual void sendAccVelTicks(short _accL, short _accR, 
				 short _velL, short _velR) = 0;
    virtual void sendAccVelTicks(short _acc, short _vel) = 0;

    //! Write binary controller commands.
    virtual void writeBinary(char const * _message, int _len) = 0;
    //! Write ascii message vectors.
    virtual void writeMessage(char const * const _message[]) = 0;
  };
}
#endif
