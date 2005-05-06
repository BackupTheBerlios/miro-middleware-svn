/*
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
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
 * $Id$
 */

#ifndef GpsService_h
#define GpsService_h

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"

#include "GpsConnection.h"
#include "GpsImpl.h"
#include "CtGpsImpl.h"

namespace GPS
{
  class EventHandler;
  class Parameters;
}

namespace Miro
{
  class GpsServer : public Server
  {
  public:
    GpsServer(int argc, char *argv[]);
    ~GpsServer();

  protected:
    //! Reference to service parameters
    const GPS::Parameters& parameters_;

    /**
     * we have put the reactor that runs in seperate task,
     * for providing concurrency.
     */
    ReactorTask reactorTask_;

    //! CORBA reference to odometry for position stamps
    CosNotifyChannelAdmin::EventChannel_var ec_;

    StructuredPushSupplier * structuredPushSupplier_;

    GPS::GpsImpl * pGps_;
    GPS::EventHandler * pEventHandler_;
    GPS::Connection * pConnection_;

    Miro::Gps_var GpsObject_;
  };
}

#endif // GpsService_h
