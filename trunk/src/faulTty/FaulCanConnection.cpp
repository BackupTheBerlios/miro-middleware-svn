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
  FaulCanConnection::writeMessage(char const * const _message)
  {
     connection2003_->writeWheel(_message, strlen(_message), motor_);
  }
};
