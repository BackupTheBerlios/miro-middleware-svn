/* -*- C++ -*-
 *
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2006 Gerald Steinbauer and Andreas Limberger <limes@sbox.tugraz.at>
 */


#include "InertialSensorEventHandler.h"
#include "XsensInertialSensorMessage.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include "idl/InertialSensorEventC.h"

#include <iostream>
#include <ace/OS.h>

namespace Inertial
{
  EventHandler::EventHandler( InertialSensorPollTask* _inertialTask ) :
       inertialTask_( _inertialTask ),
       message_( new ACE_Message_Block( sizeof ( Inertial::InertialMessage ) ) )
  {
    MIRO_LOG_CTOR("Inertial::EventHandler");
    MIRO_LOG_CTOR_END("Inertial::EventHandler");
  }

  EventHandler::~EventHandler()
  {
    MIRO_LOG_DTOR("Inertial::EventHandler");
    MIRO_LOG_DTOR_END("Inertial::EventHandler");
  }

  int EventHandler::handle_input( ACE_HANDLE fd )
  {

  }

  int EventHandler::handle_close( ACE_HANDLE, ACE_Reactor_Mask )
  {
    MIRO_LOG( LL_NOTICE, "Inertial::EventHandler handle_close called" );
    return 0;
  }

}

