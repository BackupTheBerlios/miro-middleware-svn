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

#include "SickLaserImpl.h"
#include "Parameters.h"
#include "SickLaserService.h"

#include "miro/Utils.h"
#include "miro/OdometryTracking.h"

#include <qfile.h>
#include <qdom.h>

#include <iostream>

namespace Miro
{
  using std::cerr;
  using std::endl;

  LaserServer::LaserServer(int argc, char *argv[]) :
    Miro::Server(argc, argv),
    Log(INFO,"LaserServer"),
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
      cerr << "Could not resolve event channel in naming context " 
	   << namingContextName << "," << endl
	   << "will not broadcast events.";
    }

    if (::Laser::Parameters::instance()->positionStamps && CORBA::is_nil(ec_.in())) {
      cerr << "Could not resolve event channel in naming context "
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
  
    log(INFO, "LaserServer initialized..");
  }

  LaserServer::~LaserServer()
  {
    log(INFO, "Destructing LaserServer.");
    connection_.stopTasks();
    log(INFO, "LaserServer tasks stopped.");

    // stop reactor task
    reactorTask_.cancel();

    //   laser->syncLaserScan.release();
    //   laser->syncModeChange.release();
    //   laser->syncStatus.release();
    //   laser->laserEvent->syncMutex.release(); 
    //   laser->laserEvent->stateMutex.release();
    //   laser->laserEvent->ackNackCond.broadcast();
    //   laser->laserEvent->ackNackMutex.release();
    //   reactorTask->wait ();
    log(INFO, "reactorTask exited.");

    //  delete laser;
  }

  enum {INFO, WARNING, ERROR, FATAL_ERROR};

  // TODO improve via loglevel
  void log(int logLevel, std::string s) { 
    //   cerr << __FILE__  << " : " 
    // 	 << __LINE__ << ":" 
    // 	 << __FUNCTION__<< s << endl;
    ACE_Time_Value time;
    time_t tt;

    time = ACE_OS::gettimeofday();
    tt = time.sec();
 
    std::string label[]={"[INFO]","[WARNING]","[ERROR]","[FATAL ERROR]"};
    cerr  << "[main] " << label[logLevel] << " " << s << " " << ctime(&tt);
  
  }
};

int
main(int argc, char *argv[])
{

  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Laser::Parameters * parameters = Laser::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
  config->setSection("Robot");
  config->getParameters("Robot", *robotParameters);
  config->setSection("Sick");
  config->getParameters("Laser", *parameters);
  delete config;

  log(Miro::INFO, "Initialize server daemon.");
  Miro::LaserServer laserServer(argc, argv);

#ifdef DEBUG
  cout << "  robot parameters:" << endl << *robotParameters << endl;
  cout << "  parameters:" << endl << *parameters << endl;
#endif

  try {
    log(Miro::INFO, "Ready for service.");
    laserServer.run(8);
    log(Miro::INFO, "Service loop ended, exiting.");
  }
  catch (const Miro::CException& e) {
    log(Miro::FATAL_ERROR, "Miro exception: ");
    cerr << e << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    log(Miro::FATAL_ERROR, "Uncaught CORBA exception: ");
    cerr << e << endl;
    return 1;
  }
  catch (const Miro::Exception& e) {
    log(Miro::FATAL_ERROR, "Miro exception: ");
    cerr << e << endl;
    return 1;
  }
  catch (...) {
    log(Miro::FATAL_ERROR, "Uncaught exception.");
    return 1;
  }
  return 0;
}

