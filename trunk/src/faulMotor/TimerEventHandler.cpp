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

#include "TimerEventHandler.h"
#include "FaulMotorConnection.h"

#include "sparrow/Parameters.h"

#include "miro/TimeHelper.h"
#include "miro/Log.h"

namespace FaulMotor
{
  TimerEventHandler::TimerEventHandler(Connection& _connection) :
    connection_(_connection),
    stallCounter_(0)
  {
    MIRO_LOG_CTOR("FaulMotor::TimerEventHandler");
  }

  TimerEventHandler::~TimerEventHandler()
  {
    MIRO_LOG_DTOR("FaulMotor::TimerEventHandler");
  }

  // timer callback
  int
  TimerEventHandler::handle_timeout(const ACE_Time_Value &, const void *)
  {
    if (Connection::gotTicks_ == 0) {
      ++stallCounter_;
      MIRO_LOG_OSTR(LL_WARNING, "FaulMotor::TimerEventHandler odometry stall " << stallCounter_);
      if (stallCounter_ >= 5) {
	MIRO_LOG_OSTR(LL_WARNING, "FaulMotor::TimerEventHandler resending an odometry request. " << stallCounter_);
	connection_.getTicks();
	connection_.deferredSetSpeed();
	// reset stall counter
	stallCounter_ = 0;
      }
    }
    else {
      // reset stall counter
      stallCounter_ = 0;
      connection_.getTicks();
      connection_.deferredSetSpeed();
    }

    return 0;
  }
}
