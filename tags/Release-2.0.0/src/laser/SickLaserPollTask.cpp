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
#include "SickLaserPollTask.h"
#include "SickLaserConnection.h"
#include "Parameters.h"
#include "SickLaserStatistic.h"
#include "SickLaserEvent.h"
#include "SickLaserMessage.h"

#include "miro/Log.h"

#include <netinet/in.h>

namespace Miro
{
  LaserPollTask::LaserPollTask(LaserConnection& _laser, 
			       LaserEvent& _event,
			       LaserStatistic * _laserStatistic) : 
    laser(_laser),
    event(_event),
    description(*::Laser::Parameters::instance()),
    laserStatistic(_laserStatistic)
  {
    MIRO_LOG_CTOR("miro::LaserPollTask");
  }

  LaserPollTask::~LaserPollTask() 
  {
    MIRO_LOG_DTOR("miro::LaserPollTask");
  }

  int LaserPollTask::svc() 
  {
    char c = 0x01;
    LaserMessage lm( 0, LO_REQ_DATA, 1, &c );
    ACE_Time_Value abstimeout, last, now;
    ACE_Time_Value timeout(1,0); // timeout for data request 

    MIRO_LOG_OSTR(LL_NOTICE, "polling intervall is : "<<(int)description.pollInterval.msec());
    // msg_queue is used to terminate this thread
    // when we receive a MB_HANGUP we stop
    //
    while (!canceled_) {
      last = ACE_OS::gettimeofday();

      // reqeust a reading from the laser
      laser.sendRaw(lm);
      // calc timeout
      abstimeout = ACE_OS::gettimeofday();
      abstimeout += timeout;
      { // wait for reading or timeout
	Guard guard(laser.syncLaserScan);
	if (laser.syncLaserScanCond.wait(&abstimeout)<0) {
	  if (laserStatistic)
	    laserStatistic->timeouts++;
	  event.timeout();
	  MIRO_LOG(LL_WARNING,"timeout.");
	}
      }

      // delay some time
      // TODO improve
      now = ACE_OS::gettimeofday();

      ACE_OS::sleep( description.pollInterval - (now - last) );
    }

    MIRO_LOG(LL_NOTICE,"left service");

    return (0);
  }
};
