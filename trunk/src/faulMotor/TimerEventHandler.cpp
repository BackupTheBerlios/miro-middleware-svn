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
#include "sparrow/Parameters.h"
#include "miro/TimeHelper.h"

//#undef DEBUG

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
  using std::cout;
  using std::cerr;
  using std::endl;

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
    if (Connection::gotTicks_ == 0) {
      std::cerr << " odometry stall " << ACE_OS::gettimeofday() << endl;
    }
    else {
      if (!Sparrow::Parameters::instance()->sparrow2003)
	connection_.getTicks();
      connection_.deferredSetSpeed();
    }

    return 0;
  }
}
