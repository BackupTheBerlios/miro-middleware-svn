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
#include "FaulTtyConnection.h"
#include "FaulTtyEventHandler.h"
#include "FaulTtyMessage.h"

#include "miro/Log.h"
#include "miro/Exception.h"
#include "idl/ExceptionC.h"

#include <cstring>
#include <iostream>

// #undef DEBUG
 
#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace
{
  ACE_Time_Value const TIME_OUT(0, 40000);
};

namespace FaulController
{
  using std::cout;
  using std::cerr;
  using std::endl;

  FaulTtyConnection::FaulTtyConnection(ACE_Reactor * _reactor,
			 EventHandler * _eventHandler,
			 const Miro::TtyParameters& _parameters) :
    ttyConnection_(_reactor, _eventHandler, _parameters)
  {
    DBG(cout << "Constructing FaulTtyConnection" << endl);
  }


  FaulTtyConnection::~FaulTtyConnection()
  {
    DBG(cout << "Destructing FaulTtyConnection" << endl);
  }

  void
  FaulTtyConnection::sendAccVelTicks(short, short, short, short)
  {
    MIRO_ASSERT(false);
  }

  void
  FaulTtyConnection::sendAccVelTicks(short _acc, short _vel)
  {
    char message[32];
    memset(message, 0, 32);

    int acc = _acc;
    int vel = _vel;

    sprintf(message, 
	    "ac%d\r\nv%d\r\n%c", 
	    acc, vel, 201);
  
    Miro::Guard guard(mutex_);
    int rc = ttyConnection_.ioBuffer.send_n(message, strlen(message));
    
    if (rc == -1)
      throw Miro::CException(errno, "Error writing FaulTty device.");
  }

  void
  FaulTtyConnection::writeBinary(char const * const _buffer, int _len)
  {
    ACE_Time_Value av(ACE_OS::gettimeofday() + ACE_Time_Value(1));
    if (mutex_.acquire(av) == -1)
      throw Miro::CException(errno, "Error on FaulTty mutex.");

    int rc = ttyConnection_.ioBuffer.send_n(_buffer, _len);
    mutex_.release();

    if (rc == -1)
      throw Miro::CException(errno, "Error writing FaulTty device.");
  }

  void
  FaulTtyConnection::writeMessage(char const * const _message[])
  {
    int rc = 0;

    ACE_Time_Value av(ACE_OS::gettimeofday() + ACE_Time_Value(1));
    if (mutex_.acquire(av) == -1)
      throw Miro::CException(errno, "Error on FaulTty mutex.");

//     ACE_Time_Value delta = ACE_OS::gettimeofday() - lastWrite_;
//     if (delta < TIME_OUT) {
//       // is this sleep necessary ???
//       // well, yes

//       ACE_OS::sleep(TIME_OUT - delta); // this is at least 10usec thanks to linux
//     }

    // copy the messages 
    // setting newlines inbetween
    char buffer[256];
    char * dest = buffer;
    char const * const * pMessage = _message;
    while (*pMessage ) {
      char const * src = *pMessage;
      while (*src) {
	*dest = *src;
	++dest;
	++src;
      }
      *dest = '\r';
      ++dest;
      *dest = '\n';
      ++dest;
      ++pMessage;
    }
    *dest = 0;

    unsigned int len = dest - buffer;
    if (len) {
      rc = ttyConnection_.ioBuffer.send_n(buffer, len);
      // lastWrite_ = ACE_OS::gettimeofday();
      // std::cout << "faulTTy: " << buffer << "end" << endl;
    }

    mutex_.release();

    if (rc == -1)
      throw Miro::CException(errno, "Error writing FaulTty device.");
  }
}
