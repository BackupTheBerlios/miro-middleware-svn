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
#ifndef RangeSensorBehaviour_h
#define RangeSensorBehaviour_h

#include "idl/RangeSensorC.h"
#include "botsvcs/BAP/EventBehaviour.h"

#include "Parameters.h"

#include <complex>
#include <list>
#include <map>

namespace Miro
{
  class Client;
  class StructuredPushSupplier;
};

class RangeSensorBehaviour : public Miro::BAP::EventBehaviour
{
  typedef Miro::BAP::EventBehaviour Super;

protected:
  typedef std::complex<double> Vector2d;
  struct SensorReading
  {
    ACE_Time_Value time;
    Vector2d point;

    SensorReading(const ACE_Time_Value& _time, const Vector2d& _point) :
      time(_time),
      point(_point)
    {}
  };
  typedef std::list<SensorReading> SensorScan;
  typedef std::multimap<double, Vector2d> EgoMap;

public:
  RangeSensorBehaviour(Miro::Client& _client,
		       CosNotifyChannelAdmin::EventChannel_ptr _ec,
		       const std::string& _name,
		       const std::string& _domainName,
		       Miro::StructuredPushSupplier * _pSupplier = NULL);

  void init(const Miro::BAP::BehaviourParameters * _params);
  void action();
  const std::string& getBehaviourName() const;

protected:
  void truncateBuffer(const ACE_Time_Value& _time);
  void addBuffer(const ACE_Time_Value& _time, const Vector2d& _p);
  void evalSensor(const ACE_Time_Value& _time, 
		  unsigned long group, unsigned long index, long range);
  void evalScan();

  Miro::Client& client_;
  Miro::RangeSensor_var rangeSensor_;

  Miro::Mutex mutex_;

  bool initialized_;
  double heading_;
  Vector2d position_;

  SensorScan scan_;
  EgoMap egoMap_;

  const std::string name_;
  const std::string domainName_;
  std::string sensorName_;

  Miro::ScanDescriptionIDL_var description_;

  Miro::StructuredPushSupplier * pSupplier_;
};
#endif
