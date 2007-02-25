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
	if (!Sparrow::Parameters::instance()->sparrow2003)
	  connection_.getTicks();
	connection_.deferredSetSpeed();
	// reset stall counter
	stallCounter_ = 0;
      }
    }
    else {
      // reset stall counter
      stallCounter_ = 0;
      if (!Sparrow::Parameters::instance()->sparrow2003)
	connection_.getTicks();
      connection_.deferredSetSpeed();
    }

    return 0;
  }
}
