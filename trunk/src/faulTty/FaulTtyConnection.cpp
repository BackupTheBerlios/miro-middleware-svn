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

#include "miro/Exception.h"
#include "miro/ExceptionC.h"

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
    ACE_Time_Value av(ACE_OS::gettimeofday() + ACE_Time_Value(1));

    if (mutex_.acquire(av) == -1)
      throw Miro::CException(errno, "Error writing faulTty device.");

    ACE_Time_Value t = ACE_OS::gettimeofday();
    ACE_Time_Value delta = t - lastWrite_;

    if (delta < TIME_OUT) {
      // is this sleep necessary ???
      // well, yes

      ACE_OS::sleep(TIME_OUT - delta); // this is at least 10usec thanks to linux
    }

    int rc = ioBuffer.send_n(_message, strlen(_message));
    lastWrite_ = ACE_OS::gettimeofday();

    mutex_.release();

    if (rc == -1)
      throw Miro::EDevIO("Error writing FaulTty device.");
  }
};
