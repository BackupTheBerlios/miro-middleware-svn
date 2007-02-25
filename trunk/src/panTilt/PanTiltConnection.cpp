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
#include "idl/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Synch.h"

#include "PanTiltConnection.h"
#include "PanTiltEventHandler.h"
#include "PanTiltMessage.h"
#include "PanTiltData.h"
#include "Parameters.h"

#include <ace/Reactor.h>

#include <iostream>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace DpPanTilt
{
  using Miro::ACE_Exception;
  using Miro::Guard;
  using Miro::CException;

  using std::cout;
  using std::cerr;
  using std::endl;

  Connection::Connection(ACE_Reactor * _reactor, 
			 EventHandler * _eventHandler,
			 Data& _data) :
    Super(_reactor, _eventHandler, *Parameters::instance()),
    eventHandler(_eventHandler),
    startupTimerId(-1),
    data_(_data)
  {
    DBG(cout << "Constructing Connection" << endl);

    startupTimerId = 
      reactor_->schedule_timer(eventHandler, 
			      NULL,               // timer id
			      ACE_Time_Value(0)); // delay
    if (startupTimerId == -1)
      throw ACE_Exception(errno, "Failed to register timer for startup processing.");
  }


  Connection::~Connection()
  {
    DBG(cout << "Destructing Connection" << endl);
  }

  void 
  Connection::writeMessage(Message& message)
  {
    DBG(cout << "PanTiltImpl::writeMessage: Sending message to panTilt. ");
    DBG(cout << lm << endl);

    message.addChar(' ');

    //
    // send a message
    //
    int rc = ioBuffer.send_n(message.getMessage().c_str(), message.getMessage().length());
    if (rc == -1)
      throw Miro::EDevIO("Error writing pantilt device.");

    DBG(cerr << "PanTiltClass::writeMessage: wrote "<< n << " bytes " << endl);
  }

  void 
  Connection::checkConnection() 
  {
    ACE_Time_Value abstimeout;
    ACE_Time_Value timeout(5,0); // 5 seconds for data request (far to long, but its just here to return sometime)
    Guard guard(data_.sync);

    Message lm("V");
    int failures = 0;
    int tries = 0;

    while (tries == failures && tries < 5) {
      writeMessage(lm);
      //  wait for result here
      abstimeout = ACE_OS::gettimeofday();
      abstimeout += timeout;
      if (data_.syncCond.wait(&abstimeout) < 0) {
	++failures;
      }
      if (data_.failed)
	++failures;
      ++tries;
    }

    if (tries == failures)
	throw Miro::Exception("CheckConnection timed out.");
  }
};
