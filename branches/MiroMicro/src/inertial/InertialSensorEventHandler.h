/* -*- C++ -*-
 *
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2006 Gerald Steinbauer and Andreas Limberger <limes@sbox.tugraz.at>
 */

#ifndef InertialSensorEventHandler_h
#define InertialSensorEventHandler_h

#include <ace/Event_Handler.h>
#include <ace/Synch.h>

#include "InertialSensorImpl.h"
#include "InertialSensorPollTask.h"
//#include "XsensInertialSensorConnection.h"

namespace Inertial
{
  // forward declarations
//  class XsensInertialSensorConnection;
//  class InertialSensorImpl;
//  class Parameters;
  class InertialSensorPollTask;

  class EventHandler : public virtual ACE_Event_Handler
  {

  public:
    EventHandler( InertialSensorPollTask * _inertialTask );
    virtual ~EventHandler();

    virtual int handle_input( ACE_HANDLE fd );

    virtual int handle_close( ACE_HANDLE handle, ACE_Reactor_Mask close_mask );

    //virtual int pollData( );

  private:

    InertialSensorPollTask* inertialTask_;

    ACE_Message_Block * message_;
  };
}

#endif // InertialSensorEventHandler_h

