// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "FaulTtyConnection.h"
#include "FaulTtyEventHandler.h"
#include "FaulTtyMessage.h"

#include "miro/ExceptionC.h"

#include <iostream>

// #undef DEBUG
 
#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace FaulTty
{
  Connection::Connection(ACE_Reactor * _reactor, 
			 EventHandler * _eventHandler,
			 const Miro::TtyParameters& _parameters) :
    Super(_reactor, _eventHandler, _parameters),
    eventHandler(_eventHandler)
  {
    DBG(cout << "Constructing FaulTtyConnection" << endl);
  }


  Connection::~Connection()
  {
    DBG(cout << "Destructing FaulTtyConnection" << endl);
  }

  void
  Connection::writeMessage(char const * const _message)
  {
    //    ACE_Time_Value time = ACE_OS::gettimeofday();

    int rc = ioBuffer.send_n(_message, strlen(_message));
    //    lastWrite = time;

    if (rc == -1)
      throw Miro::EDevIO("Error writing FaulTty device.");
  }
};
