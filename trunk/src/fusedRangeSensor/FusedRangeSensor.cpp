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
  
