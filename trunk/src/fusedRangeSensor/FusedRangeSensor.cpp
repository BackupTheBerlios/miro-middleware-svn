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
#include "Parameters.h"
#include "FusedRangeSensor.h"

#include "miro/Log.h"
#include "miro/Configuration.h"

#include <iostream>
#include <vector>

using std::endl;

namespace Miro
{
  FusedRangeSensorServer::FusedRangeSensorServer(int argc , char * argv[]) :
    Miro::Server(argc,argv),
    reactorTask_(this),
    ec_(tryResolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel")),
    structuredPushSupplier_(new StructuredPushSupplier(ec_.in(), namingContextName)),
    fusedImpl_(this,ec_.in(),::Fused::Parameters::instance()->fusedDescription,structuredPushSupplier_)
  {
    MIRO_LOG_CTOR("Miro::FusedRangeSensor");
    if (CORBA::is_nil(ec_.in())) {
     MIRO_LOG_OSTR(LL_ERROR, 
		    "Could not resolve event channel in naming context " <<
		    namingContextName << "," << std::endl <<
		    "will not broadcast events.");
    }

    pFusedRangeSensor = fusedImpl_._this();
    addToNameService(pFusedRangeSensor.in(), "Fused");

    reactorTask_.open(0);
    MIRO_LOG_CTOR_END("Miro::FusedRangeSensor");
  }

  FusedRangeSensorServer::~FusedRangeSensorServer()
  {
    MIRO_LOG_DTOR("Miro::FusedRangeSensor");
    reactorTask_.cancel();

    PortableServer::ObjectId_var oid = poa->reference_to_id(pFusedRangeSensor);
    poa->deactivate_object(oid.in());

    MIRO_LOG_DTOR_END("Miro::FusedRangeSensor");
  }
};

int
main(int argc, char *argv[])
{
  int rc = 0;

  Fused::Parameters * parameters = Fused::Parameters::instance();

  parameters->fusedDescription.scanType=Miro::RangeSensor::FULL;
  parameters->fusedDescription.eventName="Fused";
  parameters->fusedDescription.group.length(1);
  parameters->fusedDescription.group[0].sensor.length(0);
  parameters->fusedDescription.group[0].description.maxRange=99999;


  try {
    Miro::Log::init(argc,argv);

    MIRO_LOG(LL_NOTICE, "Initialize server daemon.\n");
    Miro::FusedRangeSensorServer fusedServer(argc,argv);

    MIRO_LOG(LL_NOTICE, "Loop forever handling events.\n");
    fusedServer.run(5);
    MIRO_LOG(LL_NOTICE, "Server loop ended, exiting.\n");
  }
  catch (const CORBA::Exception & e) {
    MIRO_LOG_OSTR(LL_CRITICAL, "Uncaught CORBA exception: " << e);
    rc = 1;
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
  
