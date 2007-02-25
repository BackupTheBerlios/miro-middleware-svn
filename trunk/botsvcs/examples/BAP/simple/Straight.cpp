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
#include "Straight.h"

#include "botsvcs/BAP/MotionArbiterMessage.h"
#include "miro/Log.h"

const std::string Straight::name_ = "Straight";

BEHAVIOUR_PARAMETERS_FACTORY_IMPL(Straight, StraightParameters)

Straight::Straight(ACE_Reactor &ar_) : 
  Super(ar_)
{
}

void 
Straight::action() 
{
  // get the parameters
  const StraightParameters * params = 
    dynamic_cast<const StraightParameters *>(params_);
  // make sure nothing went wrong
  MIRO_ASSERT(params != NULL);

  // create arbiter message
  Miro::BAP::MotionArbiterMessage message;

  // initialize arbiter message
  message.id = this;
  message.active = true;
  message.velocity.translation = params->translation;
  message.velocity.rotation = params->rotation;

  // send arbiter message
  arbitrate(message);
}

const std::string&
Straight::getBehaviourName() const 
{
  return name_;
}
