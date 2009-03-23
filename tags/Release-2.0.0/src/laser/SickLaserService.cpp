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
#include "SickLaserService.h"
#include "SickLaserImpl.h"
#include "Parameters.h"

#include "miro/OdometryTracking.h"
#include "miro/Utils.h"
#include "miro/Log.h"

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

namespace Miro
{
  LaserServer::LaserServer(int argc, char *argv[]) :
    Miro::Server(argc, argv),
    schedparams_(ACE_SCHED_FIFO, 10),
    reactorTask_( this, 20, &schedparams_),
    ec_(::Laser::Parameters::instance()->notify?
	tryResolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel") : 
	CosNotifyChannelAdmin::EventChannel::_nil()),
    structuredPushSupplier_((!CORBA::is_nil(ec_.in()))?
			    new StructuredPushSupplier(ec_.in(), namingContextName) :
			    NULL),
    odoTracking_(::Laser::Parameters::instance()->positionStamps?
		 new OdometryTracking(ec_.in(), namingContextName) :
		 NULL),
    laser_(odoTracking_, ::Laser::Parameters::instance()->laserDescription, structuredPushSupplier_),
    laserStatistic_(::Laser::Parameters::instance()->statistics?
		    new LaserStatistic() : NULL),
    connection_(reactorTask_.reactor(), laser_, laserStatistic_)
  {
    if (::Laser::Parameters::instance()->notify && CORBA::is_nil(ec_.in())) {
      MIRO_LOG_OSTR(LL_ERROR, 
		    "Could not resolve event channel in naming context " <<
		    namingContextName << "," << std::endl <<
		    "will not broadcast events.");
    }

    if (::Laser::Parameters::instance()->positionStamps && CORBA::is_nil(ec_.in())) {
      MIRO_LOG_OSTR(LL_ERROR, 
		    "Could not resolve event channel in naming context " <<
		    namingContextName << "," << std::endl <<
		    "will not use position stamps.");
    }

    if (laserStatistic_)
      laserStatistic_->reset();

    pLaser = laser_._this();
    addToNameService(pLaser.in(), "Laser");

    // start the asychronous task listening for the hardware
    reactorTask_.open(0);

    // this tries to connect to the laser
    // this _CANNOT_ be done earlier as the reactorTask is not jet running
    connection_.initHardware();
  
    MIRO_LOG_CTOR_END("Miro::LaserServer");
  }

  LaserServer::~LaserServer()
  {
    MIRO_LOG_DTOR("Miro::LaserServer");
    connection_.stopTasks();

    // stop reactor task
    reactorTask_.cancel();

    //  delete laser;
    PortableServer::ObjectId_var oid = poa->reference_to_id(pLaser);
    poa->deactivate_object(oid.in());

    MIRO_LOG_DTOR_END("Miro::LaserServer");
  }
}

void
calculateLaserDescription(Laser::Parameters * parameters)
{
  parameters->laserDescription.scanType = Miro::RangeSensor::GROUPWISE;
  parameters->laserDescription.eventName = "Laser";
  parameters->laserDescription.group.length(1);
  parameters->laserDescription.group[0].description.minRange = 50;
  parameters->laserDescription.group[0].description.maxRange = 81910;
  parameters->laserDescription.group[0].description.focus =
		Miro::deg2Rad(parameters->scanResolution);

  int values = (int)(parameters->fov / parameters->scanResolution + 1);
  parameters->laserDescription.group[0].sensor.length(values);
  for (int i = 0; i < values; ++i) {
    parameters->laserDescription.group[0].sensor[i].height = 300;
    parameters->laserDescription.group[0].sensor[i].distance = 0;
    parameters->laserDescription.group[0].sensor[i].alpha =
	Miro::deg2Rad(-(parameters->fov/2) + i * parameters->scanResolution);
    parameters->laserDescription.group[0].sensor[i].beta = 0;
    parameters->laserDescription.group[0].sensor[i].gamma = 0;
  }
}

int
main(int argc, char *argv[])
{
  int rc = 0;

  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Laser::Parameters * parameters = Laser::Parameters::instance();

  try {
    Miro::Log::init(argc, argv);
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Miro::RobotParameters", *robotParameters);
    config->setSection("Sick");
    config->getParameters("Laser::Parameters", *parameters);
    delete config;
    calculateLaserDescription(parameters);

    MIRO_LOG_OSTR(LL_NOTICE, 
		  "Configuration:\n" << 
		  "  robot parameters:" << std::endl <<
		  *robotParameters << std::endl <<
		  "  parameters:" << std::endl <<
		  *parameters);

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.\n");
    Miro::LaserServer laserServer(argc, argv);
    
    try {
      MIRO_LOG(LL_NOTICE, "Loop forever handling events.\n");
      laserServer.run(8);
      MIRO_LOG(LL_NOTICE, "Server loop ended, exiting.\n");
    }
    catch (const Miro::EOutOfBounds& e) {
      MIRO_LOG_OSTR(LL_CRITICAL, 
		    "OutOfBounds exception: Wrong parameter for device initialization."
		    << e);
      rc = 1;
    }
    catch (const Miro::EDevIO& e) {
      MIRO_LOG_OSTR(LL_CRITICAL,
		    "DevIO exception: Device access failed." 
		    << e);
      rc = 1;
    }
    catch (const CORBA::Exception & e) {
      MIRO_LOG_OSTR(LL_CRITICAL, "Uncaught CORBA exception: " << e);
      rc = 1;
    }
  }
  catch (const Miro::CException& e) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Miro C exception: " << e);
    rc = 1;
  }
  catch (const Miro::Exception& e) {
    MIRO_LOG_OSTR(LL_CRITICAL,"Miro exception: " << e);
    rc = 1;
  }
  return rc;
}

