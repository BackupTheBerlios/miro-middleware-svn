// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////


#include "SparrowEventHandler2003.h"
#include "SparrowConnection2003.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif


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
  using std::cout;

  //  Parameters* EventHandler::sparrowParam; // sparrow parameters

  EventHandler2003::EventHandler2003(Connection2003& connection_) :
    connection(connection_)

  {
    DBG(cout << "Constructing SparrowEventHandler." << endl);
    initCanConnection();
  }

  EventHandler2003::~EventHandler2003()
  {
    DBG(cout << "Destructing SparrowEventHandler." << endl);
  }

  int
  EventHandler2003::handle_timeout(const ACE_Time_Value &, const void *arg)
  {

    return 0;
  }

  void
  EventHandler2003::initCanConnection() const
  {
    DBG(cout << "Sparrow startup event." << endl);

    // initialize can connection
    connection.Can::Connection::init();
    connection.init();

    DBG(cout << "Sparrow startup successfull." << endl);
  }
};
