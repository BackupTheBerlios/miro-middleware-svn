// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "SickLaserService.h"
#include "SickLaserImpl.h"
#include "Parameters.h"

#include "miro/OdometryTracking.h"
#include "miro/Utils.h"
#include "miro/Log.h"

#include <sstream>
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
      std::cerr << "Could not resolve event channel in naming context " 
		<< namingContextName << "," << endl
		<< "will not broadcast events.";
    }

    if (::Laser::Parameters::instance()->positionStamps && CORBA::is_nil(ec_.in())) {
      std::cerr << "Could not resolve event channel in naming context "
		<< namingContextName << "," << endl
		<< "will not use position stamps." << endl;
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

    MIRO_LOG_DTOR_END("Miro::LaserServer");
  }
}

int
main(int argc, char *argv[])
{
  int rc = 0;

  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Laser::Parameters * parameters = Laser::Parameters::instance();

  try {
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", *robotParameters);
    config->setSection("Sick");
    config->getParameters("Laser", *parameters);
    delete config;

    if (Miro::Log::level() > 1) {
      std::stringstream s;
      s << "  robot parameters:" << endl << robotParameters << endl
	<< "  parameters:" << endl << parameters << endl;
      
      MIRO_LOG_OSTR(LL_NOTICE, "Configuration:\n"<< s.str().c_str());
    }

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.\n");
    Miro::LaserServer laserServer(argc, argv);
    
    try {
      MIRO_LOG(LL_NOTICE, "Loop forever handling events.\n");
      laserServer.run(8);
      MIRO_LOG(LL_NOTICE, "Server loop ended, exiting.\n");
    }
    catch (const Miro::EOutOfBounds& e) {
      std::stringstream s;
      s << "OutOfBounds exception: Wrong parameter for device initialization."
	<< e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s.str().c_str());
      rc = 1;
    }
    catch (const Miro::EDevIO& e) {
      std::stringstream s;
      s << "DevIO exception: Device access failed." 
	<< e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s.str().c_str());
      rc = 1;
    }
    catch (const CORBA::Exception & e) {
      std::stringstream s;
      s << "Uncaught CORBA exception: " << e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s.str().c_str());
      rc = 1;
    }
  }
  catch (const Miro::CException& e) {
    std::stringstream s;
    s << "Miro C exception: " << e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s.str().c_str());
    rc = 1;
  }
  catch (const Miro::Exception& e) {
    std::stringstream s;
    s << "Miro exception: " << e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s.str().c_str());
    rc = 1;
  }
  catch (...) {
    std::stringstream s;
    s << "Uncaught exception: " << endl;
    MIRO_LOG_OSTR(LL_CRITICAL, s.str().c_str());
    rc = 1;
  }
  return rc;
}

