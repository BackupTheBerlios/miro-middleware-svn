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


#include "SparrowEventHandler.h"
#include "SparrowConnection.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Sparrow
{
  using std::cout;

  //  Parameters* EventHandler::sparrowParam; // sparrow parameters

  EventHandler::EventHandler(Connection& connection_) :
    connection(connection_),
    stallTimerId(0)
  {
    DBG(cout << "Constructing SparrowEventHandler." << endl);
  }

  EventHandler::~EventHandler()
  {
    DBG(cout << "Destructing SparrowEventHandler." << endl);
  }

  int 
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *arg)
  {
    switch ((const int)arg) {
    case BUTTONS_TIMER:
      connection.readDigital();
      break;
    case STALL_TIMER:
      DBG(cout << "Sparrow stall clear." << endl);
      stallTimerId = 0;
      break;
    case INIT_TIMER: {
      // set contiuous mode for status reports
      if (connection.boardReply == -1) // init
	initSparrowBoard();
      else if (connection.boardReply == 0) {
	std::cerr << endl << endl << "!!!!! SparrowBoard died!" << endl << endl;
	abort();  // board died!
      }
      else if (connection.boardReply == 1) // test
	connection.boardReply = 0;
      break;
    }
    default: 
      throw Miro::Exception("PSOSEventHandler: Unknown timeout type.");
    }
    return 0;
  }

  void
  EventHandler::initSparrowBoard() const
  {
    DBG(cout << "Sparrow startup event." << endl);

    // initialize can connection
    connection.Can::Connection::init();
    connection.init();

    Parameters * params = Parameters::instance();

    connection.getPosition(params->odometryInterval, 0);
    connection.infraredGet(params->infraredInterval, 0);

    DBG(cout << "Sparrow startup successfull." << endl);
  }
};
