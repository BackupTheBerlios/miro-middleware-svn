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
#include "Wander.h"

#include "miro/Log.h"

namespace {
  // get random number between 0 and 1
  inline
  double 
  rand01() {
    return ((double)rand()) / ((double)RAND_MAX);
  }
};

const std::string Wander::name_ = "Wander";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(Wander, WanderParameters);

Wander::Wander(ACE_Reactor &ar_) : 
  Super(ar_)
{
  // initialise the arbitration message
  message_.id = this;
  message_.active = true;
  message_.velocity.translation = 100;
  message_.velocity.rotation = 0.;
}

void 
Wander::action() 
{
  // get parameters
  const WanderParameters * params = 
    dynamic_cast<const WanderParameters *>(params_);
  // make sure nothing went wrong
  MIRO_ASSERT(params != NULL);

  // set the arbiter message to random values
  double r = rand01();
  if (r < params->thresholdChange) {
    r = rand01();
    message_.velocity.translation = 
      params->minTranslation + 
      (long)(r * (params->maxTranslation - params->minTranslation));
    r = rand01();
    if (r > params->thresholdStraight) {
      r = rand01();
      message_.velocity.rotation = 
	- params->maxRotation + r * 2 * params->maxRotation;
    }
    else
      message_.velocity.rotation = 0.;
  }
  arbitrate(message_);
}

const std::string&
Wander::getBehaviourName() const 
{
  return name_;
}
