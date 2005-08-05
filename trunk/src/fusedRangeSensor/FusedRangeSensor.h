// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef FusedRangeSensor_h
#define FusedRangeSensor_h

#include "FusedRangeSensorImpl.h"

#include "idl/FusedRangeSensorC.h"
#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"

namespace Miro
{
  class FusedRangeSensorServer : public Server
  {
    typedef FusedRangeSensorServer Self;

  public:
    //! Constructor
    FusedRangeSensorServer(int argc, char * argv[]);

    //! Destructor
    ~FusedRangeSensorServer();

  protected:
  
    ReactorTask reactorTask_;

    CosNotifyChannelAdmin::EventChannel_var ec_;

    StructuredPushSupplier * structuredPushSupplier_;

    FusedRangeSensorImpl fusedImpl_;

    Miro::FusedRangeSensor_var pFusedRangeSensor;
  };

};

#endif
