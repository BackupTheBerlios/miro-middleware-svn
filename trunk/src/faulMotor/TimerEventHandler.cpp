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

#include "TimerEventHandler.h"
#include "FaulMotorConnection.h"

#undef DEBUG

#ifdef DEBUG
#include <iostream>
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace FaulMotor
{
  TimerEventHandler::TimerEventHandler(Connection& _connection) :
    connection_(_connection)
  {
    DBG(std::cout << "Constructing FaulTty::TimerEventHandler." << endl);
  }

  TimerEventHandler::~TimerEventHandler()
  {
    DBG(std::cout << "Destructing FaulTty::TimerEventHandler." << endl);
  }

  // timer callback
  int
  TimerEventHandler::handle_timeout(const ACE_Time_Value &, const void *)
  {

    if (Connection::gotTicks_ == 0)
      std::cerr << "odometry stall" << endl;
    Connection::gotTicks_ = 0;
    return 0;
  }
};
