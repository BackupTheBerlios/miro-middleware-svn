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

#include "FaulCanConnection.h"

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
  FaulCanConnection::FaulCanConnection(Sparrow::Connection2003 * _connection2003, int _motor) :
    connection2003_(_connection2003),
    motor_(_motor)
  {
    DBG(cout << "Constructing FaulCanConnection" << endl);

  }


  FaulCanConnection::~FaulCanConnection()
  {
    DBG(cout << "Destructing FaulCanConnection" << endl);

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
};
