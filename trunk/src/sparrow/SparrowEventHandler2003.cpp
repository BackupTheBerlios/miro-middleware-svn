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
      MIRO_DBG(SPARROW, LL_DEBUG, "Sparrow::EventHandler2003 startup event.");
      connection_.init();
      MIRO_DBG(SPARROW, LL_DEBUG, "Sparrow::EventHandler2003 startup successfull.");
      break;
    case PAN_CALIBRATION_TIMER:
      connection_.queryPanTicksPerDegree();
      break;
    default:
      MIRO_LOG_OSTR(LL_ERROR,
		    "Sparrow::EventHandler2003 - Unknown timer type: " << (int)task);
    }
    
    return 0;
  }
}
