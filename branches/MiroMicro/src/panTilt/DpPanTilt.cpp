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
#include "DpPanTilt.h"

#include "PanTiltImpl.h"
#include "PanTiltData.h"
#include "PanTiltConsumer.h"
#include "PanTiltConnection.h"
#include "Parameters.h"

#include "miro/Configuration.h"
#include "miro/ConfigDocument.h"
#include "miro/Log.h"

#include <sstream>

using DpPanTilt::Parameters;
using DpPanTilt::PanTiltServer;
using std::endl;

using std::endl;
using std::cout;


namespace DpPanTilt
{
  using Miro::ReactorTask;

  PanTiltServer::PanTiltServer(int& argc, char *argv[],Miro::PanTiltParameters panTiltParameters) :
    Super(argc, argv),
    reactorTask(this),
    data_(),
    pEventHandler_(new EventHandler(connection_, new Consumer(data_))),
    connection_(reactorTask.reactor(), pEventHandler_, data_),
    panTilt(new PanTiltImpl(connection_, data_, panTiltParameters))
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
  int rc = 1;

  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Parameters * params = Parameters::instance();
  Miro::PanTiltParameters panTiltParameters;

  try {
    Miro::Log::init(argc, argv);
    Miro::Configuration::init(argc, argv);

    // Config file processing
    Miro::ConfigDocument * config = Miro::Configuration::document();
    config->setSection("Robot");
    config->getParameters("Miro::RobotParameters", *robotParameters);
    config->setSection("DirectedPerception");
    config->getParameters("DpPanTilt::Parameters", *params);
    config->setSection("Camera");
    config->getParameters("Miro::PanTiltParameters", panTiltParameters);

    MIRO_LOG_OSTR(LL_NOTICE, 
		  "Configuration:\n" << 
		  "  robot parameters:" << std::endl <<
		  *robotParameters << std::endl <<
		  "  pantilt paramters:" << endl <<
		  *params);
    
    MIRO_LOG(LL_NOTICE, "Initialize server daemon.");
    PanTiltServer panTiltServer(argc, argv, panTiltParameters);

    try {
      MIRO_LOG(LL_NOTICE, "Loop forever handling events.\n");
      panTiltServer.run(4);
      MIRO_LOG(LL_NOTICE, "Service loop ended, exiting.");
      rc = 0;
    }
    catch (const Miro::EOutOfBounds& e) {
      MIRO_LOG_OSTR(LL_CRITICAL, 
		    "OutOfBounds exception: Wrong parameter for device initialization."
		    << e);
    }
    catch (const Miro::EDevIO& e) {
      MIRO_LOG_OSTR(LL_CRITICAL, 
		    "DevIO exception: Device access failed." 
		    << e);
    }
    catch (const CORBA::Exception & e) {
      MIRO_LOG_OSTR(LL_CRITICAL, "Uncaught CORBA exception: " << e);
    }
  }
  catch (const Miro::CException& e) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Miro C exception: " << e);
    rc = 1;
  }
  catch (const Miro::Exception& e) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Miro exception: " << e);
    rc = 1;
  }
  return rc;
}

