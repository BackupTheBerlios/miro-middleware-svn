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

#include "DpPanTilt.h"

#include "PanTiltImpl.h"
#include "PanTiltData.h"
#include "PanTiltConsumer.h"
#include "PanTiltConnection.h"
#include "Parameters.h"

#include "miro/Utils.h"
#include "miro/Log.h"

#include <sstream>

using Miro::ConfigDocument;
using DpPanTilt::Parameters;
using DpPanTilt::PanTiltServer;

namespace DpPanTilt
{
  using Miro::ReactorTask;

  PanTiltServer::PanTiltServer(int& argc, char *argv[]) :
    Super(argc, argv),
    reactorTask(this),
    data_(),
    pEventHandler_(new EventHandler(connection_, new Consumer(data_))),
    connection_(reactorTask.reactor(), pEventHandler_, data_),
    panTilt(new PanTiltImpl(connection_, data_))
  {
    MIRO_LOG_CTOR("PanTiltServer");

    Miro::DirectedPerceptionPanTilt_var pPanTilt(panTilt->_this());
    addToNameService(pPanTilt.in(), "PanTilt");

    // start the asychronous task listening for the hardware
    reactorTask.open(0);

    MIRO_LOG_CTOR_END("PanTiltServer");
  }

  PanTiltServer::~PanTiltServer()
  {
    // stop reactor task
    reactorTask.cancel();
  }
};

int main(int argc, char *argv[])
{
  int rc = 0;

  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Parameters * params = Parameters::instance();

  try {
    // Config file processing
    ConfigDocument * config = new ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Robot", *robotParameters);
    config->setSection("DirectedPerception");
    config->getParameters("PanTilt", *params);
    delete config;

    if (Miro::Log::level() > 1) {
      std::stringstream s;
      s << "  robot parameters:" << endl << *robotParameters << endl
	<< "  pantilt paramters:" << endl << *params << endl;

      MIRO_LOG_OSTR(LL_NOTICE, "Configuration:\n" << s);
    }

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.");
    PanTiltServer panTiltServer(argc, argv);

    try {
      MIRO_LOG(LL_NOTICE, "Loop forever handling events.\n");
      panTiltServer.run(4);
      MIRO_LOG(LL_NOTICE, "Service loop ended, exiting.");
    }
    catch (const Miro::EOutOfBounds& e) {
      std::stringstream s;
      s << "OutOfBounds exception: Wrong parameter for device initialization."
	<< e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s);
      rc = 1;
    }
    catch (const Miro::EDevIO& e) {
      std::stringstream s;
      s << "DevIO exception: Device access failed." 
	<< e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s);
      rc = 1;
    }
    catch (const CORBA::Exception & e) {
      std::stringstream s;
      s << "Uncaught CORBA exception: " << e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s);
      rc = 1;
    }
  }
  catch (const Miro::CException& e) {
    std::stringstream s;
    s << "Miro C exception: " << e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s);
    rc = 1;
  }
  catch (const Miro::Exception& e) {
    std::stringstream s;
    s << "Miro exception: " << e << endl;
      MIRO_LOG_OSTR(LL_CRITICAL, s);
    rc = 1;
  }
  catch (...) {
    std::stringstream s;
    s << "Uncaught exception: " << endl;
    MIRO_LOG_OSTR(LL_CRITICAL, s);
    rc = 1;
  }
  return rc;
}

