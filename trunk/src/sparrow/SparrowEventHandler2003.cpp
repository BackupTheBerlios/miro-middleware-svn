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
  EventHandler2003::EventHandler2003(Connection2003& connection_) :
    connection(connection_)
  {
    MIRO_LOG_CTOR("Sparrow:EventHandler2003");
  }

  EventHandler2003::~EventHandler2003()
  {
    MIRO_LOG_DTOR("Sparrow:EventHandler2003");
  }

  int
  EventHandler2003::handle_timeout(const ACE_Time_Value &, const void * /*arg*/)
  {
    initCanConnection();
    return 0;
  }

  void
  EventHandler2003::initCanConnection() const
  {
    MIRO_DBG(SPARROW, LL_DEBUG, "Sparrow::EventHandler2003 startup event.");

    // initialize can connection
    connection.init();

    MIRO_DBG(SPARROW, LL_DEBUG, "Sparrow::EventHandler2003 startup successfull.");
  }
}
