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
