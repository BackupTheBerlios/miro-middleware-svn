/*
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2006 Andreas Limberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: InertialService.cpp,v 1.0 2006/03/02 14:06:00 limberger Exp $
 */

#include "InertialService.h"
#include "InertialSensorImpl.h"
#include "XsensInertialSensorConnection.h"
#include "InertialSensorEventHandler.h"
#include "Parameters.h"

#include "idl/ExceptionC.h"
#include "miro/Utils.h"
#include "miro/Configuration.h"
#include "miro/Log.h"

#include <iostream>

namespace Miro
{
  InertialServer::InertialServer(int argc, char *argv[]) :
    Miro::Server(argc, argv),
    parameters_(* Inertial::InertialParameters::instance()),
    reactorTask_(this, 20, NULL),
    ec_(tryResolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel")),
    structuredPushSupplier_(CORBA::is_nil(ec_.in())?
            NULL : new Miro::StructuredPushSupplier(ec_.in(), namingContextName))
  {
    MIRO_LOG_CTOR("Miro::InertialServer");
    if (CORBA::is_nil(ec_.in()))
      MIRO_LOG_OSTR(LL_WARNING,
              "Could not resolve EventChannel in naming context " <<
              namingContextName << "," << std::endl <<
              "will not broadcast events.");

    pInertial_     = new Inertial::InertialSensorImpl(parameters_, structuredPushSupplier_);
    pConnection_ = new Inertial::XsensInertialSensorConnection( *reactorTask_.reactor(), *pInertial_ );
//    ppollTask_     = new Inertial::InertialSensorPollTask( *pEventHandler_, *pConnection_, *pInertial_ );
//    pEventHandler_ = new Inertial::EventHandler( ppollTask_ );

    InertialSensorObject_ = pInertial_->_this();
    addToNameService(InertialSensorObject_.in(), "Inertial");

    reactorTask_.open( 0 );

    //pConnection_->setSensorMode( );

    MIRO_LOG_CTOR_END("Miro::InertialServer");
  }

  InertialServer::~InertialServer()
  {
    MIRO_LOG_DTOR("Miro::InertialServer");

    reactorTask_.cancel();

    PortableServer::ObjectId_var oid;
    oid = poa->reference_to_id(InertialSensorObject_.in());
    poa->deactivate_object(oid.in());

    MIRO_LOG_DTOR_END("Miro::InertialServer");
  }
}

int main(int argc, char *argv[])
{
  int rc = 0;
  std::cout << "start inertial service" << std::endl;

  try
  {
    Miro::Log::init(argc, argv);
    Miro::Configuration::init(argc, argv);

    Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
    Inertial::InertialParameters * parameters = Inertial ::InertialParameters::instance();

    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("Robot");
    config->getParameters("Miro::RobotParameters", *robotParameters);
//    config->setSection("Inertial");
//    config->getParameters("InertialParameters", *parameters);
    delete config;

    MIRO_LOG_OSTR(LL_NOTICE,
                  "Configuration:\n" <<
                  "  robot parameters:" << std::endl ); // <<
//                  *robotParameters << std::endl <<
//                  "  parameters:" << std::endl <<
//                  *parameters);

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.\n");
    Miro::InertialServer inertialServer(argc, argv);

    try {
      MIRO_LOG(LL_NOTICE, "Loop forever handling events.\n");
      inertialServer.run(8);
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

