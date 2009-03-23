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
#include "SparrowEventHandler2003.h"
#include "SparrowConnection2003.h"

#include "miro/Log.h"


// Roland: Mittelfristig sollte sich das aber auch mit
// Sparrow::EventHander unifizieren lassen
//
// Sparrow::Connection
//   \---> SparrowConnection99
//   \---> SparrowConnection2003
// Sparrow::Consumer
//   \---> SparrowConsumer99
//   \---> SparrowConsumer2003
// ...

namespace Sparrow
{
  EventHandler2003::EventHandler2003(Connection2003& _connection) :
    connection_(_connection)
  {
    MIRO_LOG_CTOR("Sparrow::EventHandler2003");
  }

  EventHandler2003::~EventHandler2003()
  {
    MIRO_LOG_DTOR("Sparrow::EventHandler2003");
  }

  int
  EventHandler2003::handle_timeout(ACE_Time_Value const &, void const * _arg)
  {
    unsigned int task = *reinterpret_cast<unsigned int const *>(_arg);
    switch (task) {
    case INIT_TIMER:
      MIRO_LOG(LL_NOTICE, "Sparrow::EventHandler2003 startup event.");
      connection_.init();
      MIRO_DBG(SPARROW, LL_DEBUG, "Sparrow::EventHandler2003 startup successfull.");
      break;
    case PAN_CALIBRATION_TIMER:
      MIRO_LOG(LL_NOTICE, "Sparrow::EventHandler2003 query pan ticks event.");
      connection_.queryPanTicksPerDegree();
      break;
    default:
      MIRO_LOG_OSTR(LL_ERROR,
		    "Sparrow::EventHandler2003 - Unknown timer type: " << (int)task);
    }
    
    return 0;
  }
}
