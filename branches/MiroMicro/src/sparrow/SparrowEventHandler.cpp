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
#include "SparrowEventHandler.h"
#include "SparrowConnection.h"

#include "miro/Log.h"

namespace Sparrow
{
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
    switch ((const long)arg) {
    case BUTTONS_TIMER:
      connection.readDigital();
      break;
    case STALL_TIMER:
      stallTimerId = 0;
      break;
    case INIT_TIMER: {
      // set contiuous mode for status reports
      if (connection.boardReply < 0) { // init
	MIRO_LOG(LL_NOTICE, "Sparrow::EventHandler SparrowBoard connect.");
	initSparrowBoard();
      }
      else if (connection.boardReply <= -3) {
	MIRO_LOG(LL_CRITICAL, "Sparro::EventHandler SparrowBoard died!");
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
    connection.init();

    Parameters * params = Parameters::instance();

    // get odometry only if this is not taken care of by the
    // faulhaber motor controller
    if (!params->faulhaber)
      connection.getPosition(params->odometryPulse.msec(), 0);
    connection.infraredGet(params->infraredPulse.msec(), 0);
  }
}
