/* -*- C++ -*-
 *
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2006 Gerald Steinbauer and Andreas Limberger <limes@sbox.tugraz.at>
 */


#include <cmath>
#include <ace/Reactor.h>
#include <ace/OS.h>
#include <string>

#include "InertialSensorConnection.h"
#include "InertialSensorEventHandler.h"
#include "Parameters.h"

#include "miro/Exception.h"
#include "miro/Log.h"

namespace Inertial
{
  Connection::Connection(ACE_Reactor * _reactor,
          EventHandler * _eventHandler,
          const InertialParameters& _parameters) :
    Super(*_reactor, *_eventHandler, _parameters ),
    eventHandler(_eventHandler)
  {
    MIRO_LOG_CTOR("InertialSensor::Connection");
    MIRO_LOG_CTOR_END("InertialSensor::Connection");
  }

  Connection::~Connection()
  {
    MIRO_LOG_DTOR("InertialSensor::Connection");
    MIRO_LOG_DTOR_END("InertialSensor::Connection");
  }

  void Connection::send(const std::string command)
  {
    MIRO_LOG_OSTR(LL_PRATTLE, "Sending InertialSensor command: " << command);
    std::string cmd = command + "\r\n";
    ioBuffer.send_n(cmd.c_str(), cmd.length());
  }
}


