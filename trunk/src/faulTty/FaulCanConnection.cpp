// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "FaulCanConnection.h"

#include "miro/Log.h"
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

  FaulCanConnection::FaulCanConnection(Sparrow::Connection2003 * _connection2003, 
				       int _motor) :
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
