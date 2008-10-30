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
 * $Id: InertialService.h,v 1.0 2006/03/01 14:00:00 limberger Exp $
 */

#ifndef InertialService_h
#define InertialService_h

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"

#include "InertialSensorImpl.h"
#include "InertialSensorPollTask.h"
#include "XsensInertialSensorConnection.h"


namespace Miro
{
  class EventHandler;
  class InertialParameters;

  class InertialServer : public Miro::Server
  {
  public:
    InertialServer(int argc, char *argv[]);
    ~InertialServer();

  protected:
    //! Reference to service parameters
    const Inertial::InertialParameters& parameters_;

    /**
     * we have put the reactor that runs in seperate task,
     * for providing concurrency.
     */
    Miro::ReactorTask reactorTask_;

    //! CORBA reference to odometry for position stamps
    CosNotifyChannelAdmin::EventChannel_var ec_;

    Miro::StructuredPushSupplier * structuredPushSupplier_;

    Inertial::InertialSensorImpl * pInertial_;
//    Inertial::EventHandler * pEventHandler_;
//    Inertial::InertialSensorPollTask * ppollTask_;
    Inertial::XsensInertialSensorConnection * pConnection_;

    Miro::InertialSensor_var InertialSensorObject_;
  };
}

#endif // GpsService_h
