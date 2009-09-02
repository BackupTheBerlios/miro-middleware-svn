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
#include "MotionImpl.h"

namespace Miro
{
  /**
   * @param _params Reference to the MotionParameters for the
   * motion device.
   */
  MotionImpl::MotionImpl(const MotionParameters& _params) :
      params_(_params)
  {
    targetVelocity_.translation = 0;
    targetVelocity_.rotation = 0.;
  }

  void
  MotionImpl::getMinMaxVelocity(CORBA::Long& minTranslation,
                                CORBA::Long& maxTranslation,
                                double& minRotation,
                                double& maxRotation) throw()
  {
    minTranslation = params_.minTranslation;
    maxTranslation = params_.maxTranslation;
    minRotation = params_.minRotation;
    maxRotation = params_.maxRotation;
  }

  VelocityIDL
  MotionImpl::getTargetVelocity() throw()
  {
    Guard guard(mutex_);
    return targetVelocity_;
  }

  void
  MotionImpl::setTargetVelocity(const VelocityIDL& _velocity)
  {
    targetVelocity_ = _velocity;
  }
};
