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
#ifndef LaserAvoid_h
#define LaserAvoid_h

#include "idl/RangeSensorC.h"
#include "botsvcs/BAP/TimedBehaviour.h"

#include "Parameters.h"

class LaserAvoid : public Miro::BAP::TimedBehaviour
{
  typedef Miro::BAP::TimedBehaviour Super;

public:
  LaserAvoid(ACE_Reactor& _reactor, Miro::RangeSensor_ptr _laser);

  BEHAVIOUR_PARAMETERS_FACTORY(LaserAvoidParameters);

  void action();
  const std::string& getBehaviourName() const;

protected:
  Miro::RangeSensor_var laser_;

  static const std::string name_;
};
#endif
