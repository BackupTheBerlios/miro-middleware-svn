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
#include "FaulCanConnection.h"

#include "miro/Log.h"

namespace
{
  ACE_Time_Value const TIME_OUT(0, 40000);
};

namespace FaulController
{
  FaulCanConnection::FaulCanConnection(Sparrow::Connection2003 * _connection2003, 
				       int _motor) :
    connection2003_(_connection2003),
    motor_(_motor)
  {
    MIRO_LOG_CTOR("FaulController::FaulCanConnection");
  }


  FaulCanConnection::~FaulCanConnection()
  {
    MIRO_LOG_DTOR("FaulController::FaulCanConnection");
  }
  
  void 
  FaulCanConnection::sendAccVelTicks(short, short)
  {
    MIRO_ASSERT(false);
  }

  void 
  FaulCanConnection::sendAccVelTicks(short accL, short accR, 
				     short velL, short velR)
  {
    connection2003_->writeAccVel(accL, accR, velL, velR);
  }


  void
  FaulCanConnection::writeBinary(char const * buffer, int _len)
  {
    ACE_Time_Value av(ACE_OS::gettimeofday() + ACE_Time_Value(1));

    if (mutex_.acquire(av) == -1)
      throw Miro::CException(errno, "Error writing faulCan device.");

    ACE_Time_Value t = ACE_OS::gettimeofday();
    ACE_Time_Value delta = t - lastWrite_;

    if (delta < TIME_OUT) {
      // is this sleep necessary ???
      // well, yes

      ACE_OS::sleep(TIME_OUT - delta); // this is at least 10usec thanks to linux
    }

    int index = 0;
    while (_len > index) {
      connection2003_->writeWheel(buffer + index, std::min(8, _len - index), motor_);
      index += 8;
    }
    mutex_.release();
  }


  void
  FaulCanConnection::writeMessage(char const * const _message[])
  {
    ACE_Time_Value av(ACE_OS::gettimeofday() + ACE_Time_Value(1));

    if (mutex_.acquire(av) == -1)
      throw Miro::CException(errno, "Error writing faulCan device.");

    ACE_Time_Value t = ACE_OS::gettimeofday();
    ACE_Time_Value delta = t - lastWrite_;

    if (delta < TIME_OUT) {
      // is this sleep necessary ???
      // well, yes

      ACE_OS::sleep(TIME_OUT - delta); // this is at least 10usec thanks to linux
    }

    char const * const * pMessage = _message;
    while (*pMessage) {
      connection2003_->writeWheel(*pMessage, strlen(*pMessage), motor_);
      ++pMessage;
    }
    mutex_.release();
  }
}
