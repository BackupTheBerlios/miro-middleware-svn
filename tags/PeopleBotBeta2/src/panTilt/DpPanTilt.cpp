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

#include "DpPanTilt.h"

#include "PanTiltImpl.h"
#include "PanTiltData.h"
#include "PanTiltConsumer.h"
#include "PanTiltConnection.h"
#include "Parameters.h"

#include "miro/Utils.h"
#include "miro/Log.h"

#include <qfile.h>
#include <qdom.h>

#include <iostream>

#include <sched.h>

using Miro::ConfigDocument;
using DpPanTilt::Parameters;
using DpPanTilt::PanTiltServer;

enum {INFO, WARNING, ERROR, FATAL_ERROR};

namespace DpPanTilt
{
  using Miro::Server;
  using Miro::ReactorTask;
  using Miro::Log;

  PanTiltServer::PanTiltServer(int argc, char *argv[]) :
    Server(argc, argv),
    Log(INFO,"PanTiltServer"),
    reactorTask(this),
    data_(),
    pEventHandler_(new EventHandler(connection_, new Consumer(data_))),
    connection_(reactorTask.reactor(), pEventHandler_, data_),
    panTilt(new PanTiltImpl(connection_, data_))
  {
    CosNaming::Name n;


    Miro::DirectedPerceptionPanTilt_var pPanTilt(panTilt->_this());

    n.length(1);
    n[0].id = CORBA::string_dup("PanTilt");
    if (rebind_) {
      // Force binding of controller reference to make
      // unbind first, if already bound
      // sure it is always up-to-date.

      try {
	namingContext->unbind(n);
	log (INFO,"PanTilt was still bound in naming service.");
      } catch (...) {
      }
    }
    try {
      namingContext->bind(n, pPanTilt.in());
    }
    catch (CosNaming::NamingContext::AlreadyBound& ) {
      cerr << "Objects were still bound in naming service: " 
	   << n[0].id << endl
	   << "Use -MiroRebindIOR if you really want to rebind them." << endl;
      
      throw(0);
    }

    // start the asychronous task listening for the hardware
    reactorTask.open(0);

    log(INFO, "PanTiltServer initialized..");

  }

  PanTiltServer::~PanTiltServer()
  {
    CosNaming::Name n;
    n.length(1);
    n[0].id = CORBA::string_dup("PanTilt");
    namingContext->unbind(n);
    log(INFO, "unbound name service.");

    // stop reactor task
    reactorTask.cancel();
    log(INFO, "reactorTask exited.");
  }
};

namespace 
{
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

int main(int argc, char *argv[])
{
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Parameters * params = Parameters::instance();

  // Config file processing
  ConfigDocument * config = new ConfigDocument(argc, argv);
  config->setSection("Robot");
  config->getParameters("Robot", *robotParameters);
  config->setSection("DirectedPerception");
  config->getParameters("PanTilt", *params);
  delete config;

#ifdef DEBUG
  cout << "  robot parameters:" << endl << *robotParameters << endl;
  cout << "  paramters:" << endl << *params << endl;
#endif

  log(INFO, "Initialize server daemon.");
  PanTiltServer panTiltServer(argc, argv);
  try {
    log(INFO, "Ready for service.");
    panTiltServer.run(4);
    log(INFO, "Service loop ended, exiting.");
  }
  catch (const CORBA::Exception & e) {
    log(FATAL_ERROR, "Uncaught CORBA exception: ");
    cerr << e << endl;
    return 1;
  }
  catch (...) {
    log(FATAL_ERROR, "Uncaught exception.");
    return 1;
  }
  return 0;
}

