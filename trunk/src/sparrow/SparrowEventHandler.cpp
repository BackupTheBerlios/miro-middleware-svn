// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "SparrowEventHandler.h"
#include "SparrowConnection.h"

#include "miro/Log.h"

namespace Sparrow
{
  using std::cout;
  using std::cerr;
  using std::endl;

  //  Parameters* EventHandler::sparrowParam; // sparrow parameters

  EventHandler::EventHandler(Connection& connection_) :
    connection(connection_),
    stallTimerId(0)
  {
    MIRO_LOG_CTOR("Sparrow::EventHandler");
  }

  EventHandler::~EventHandler()
  {
    MIRO_LOG_CTOR("Sparrow::EventHandler");
  }

  int 
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *arg)
  {
    switch ((const int)arg) {
    case BUTTONS_TIMER:
      connection.readDigital();
      break;
    case STALL_TIMER:
      stallTimerId = 0;
      break;
    case INIT_TIMER: {
      // set contiuous mode for status reports
      if (connection.boardReply < 0) { // init
	std::cerr << endl << endl << "!!!!! SparrowBoard connect" << endl << endl;
	initSparrowBoard();
      }
      else if (connection.boardReply <= -3) {
	std::cerr << endl << endl << "!!!!! SparrowBoard died!" << endl << endl;
	abort();  // board died!
      }
      else { // test
	--connection.boardReply;
      }
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
    // initialize can connection
    connection.Can::Connection::init();
    connection.init();

    Parameters * params = Parameters::instance();

    // get odometry only if this is not taken care of by the
    // faulhaber motor controller
    if (!params->faulhaber)
      connection.getPosition(params->odometryPulse.msec(), 0);
    connection.infraredGet(params->infraredPulse.msec(), 0);
  }
}
