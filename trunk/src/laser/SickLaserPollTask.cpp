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


#include "SickLaserPollTask.h"

#include "SickLaserConnection.h"
#include "SickLaserParameters.h"
#include "SickLaserStatistic.h"
#include "SickLaserEvent.h"
#include "SickLaserMessage.h"

#include <iostream>

#include <netinet/in.h>

#define DBG(x)

namespace Miro
{
  LaserPollTask::LaserPollTask(LaserConnection& _laser, 
			       LaserEvent& _event,
			       const ::Laser::Parameters& _description,
			       LaserStatistic * _laserStatistic) : 
    Log(INFO,"LaserPollTask"), 
    laser(_laser),
    event(_event),
    description(_description),
    laserStatistic(_laserStatistic)
  {
  }

  LaserPollTask::~LaserPollTask() 
  {
  }

  int LaserPollTask::svc() 
  {
    char c = 0x01;
    int pollintervall = description.pollintervall;
    LaserMessage lm( 0, LO_REQ_DATA, 1, &c );
    ACE_Time_Value abstimeout, last, now;
    ACE_Time_Value timeout(1,0); // timeout for data request 

    log(INFO,"polling intervall is :", pollintervall);
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
	  log(WARNING,"timeout.");
	}
      }

      // delay some time
      // TODO improve
      now = ACE_OS::gettimeofday();

      ACE_OS::sleep( ACE_Time_Value(0,pollintervall) - (now - last) );
    }

    log(INFO,"left service");

    return (0);
  }
};
