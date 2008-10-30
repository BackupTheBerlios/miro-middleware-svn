/* -*- C++ -*-
 *
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2006 Gerald Steinbauer and Andreas Limberger <limes@sbox.tugraz.at>
 */


#ifndef InertialSensorConnection_h
#define InertialSensorConnection_h

#include "Parameters.h"
#include "miro/TtyConnection.h"

#include <string>

namespace Inertial
{
  // forward declaration
  class EventHandler;

  /**
   * GPS::Connection
   */
  class Connection : public Miro::TtyConnection
  {
    typedef Miro::TtyConnection Super;

  public:
    //! Constructor
    Connection(ACE_Reactor * _reactor,
            EventHandler * _eventHandler,
            const InertialParameters& _parameters = *InertialParameters::instance());

    //! Destructor
    virtual ~Connection();
    virtual void readData(const std::string command);

  protected:
    EventHandler* eventHandler;
  };
}

#endif // InertialSensorConnection_h
