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
#ifndef SparrowConsumer2003_h
#define SparrowConsumer2003_h

#include "SparrowDevice.h"
#include "AliveCollector.h"
#include "faulMotor/FaulMotorConsumer.h"
#include "sparrow/Parameters.h"

#include "miro/Synch.h"
#include "miro/DevConsumer.h"
#include "miro/OdometryImpl.h"

// forward decleration
namespace Miro
{
  class RangeSensorImpl;
}

namespace Sparrow
{
  // forward declerations
  class Connection2003;
  class PanTiltImpl;

  class Consumer2003 : public Miro::DevConsumer
  {
    typedef Miro::DevConsumer Super;

    typedef std::vector<long> SensorVector;
    typedef std::vector<SensorVector> GroupVector;
    typedef std::vector<GroupVector> IrValues;
    typedef std::vector<int> TimeIndex;

  public:
    Consumer2003(Connection2003 * _connection,
	     Miro::OdometryImpl * _pOdometry,
	     Miro::RangeSensorImpl * _pIR1);
    Consumer2003();
    ~Consumer2003();

    virtual void handleMessage(const Miro::DevMessage * _message);
    virtual void registerInterfaces(Connection2003 * _connection,
				    Miro::OdometryImpl * _pOdometry,
				    Miro::RangeSensorImpl * _pIR1,
				    FaulMotor::Consumer * _faulConsumer,
				    PanTiltImpl * _panTilt,
				    AliveCollector * _aliveCollector);

    static const int INTEGRATION_NUMBER;

  protected:
    long integrateIrValues(unsigned int group, unsigned int sensor, long value);

    Parameters const * params_;

    Connection2003 * connection_;
    Miro::OdometryImpl * pOdometry_;
    Miro::RangeSensorImpl * pIR1_;
    PanTiltImpl * panTilt_;

    IrValues irValues_;
    TimeIndex timeIndex_;

    Miro::MotionStatusIDL status_;
    FaulMotor::Consumer * faulConsumer;

    AliveCollector * pAliveCollector;
  };
}
#endif // SparrowConsumer2003_h

