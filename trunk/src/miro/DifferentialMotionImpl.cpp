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
#include "DifferentialMotionImpl.h"

namespace Miro
{
  DifferentialMotionImpl::DifferentialMotionImpl(const DifferentialMotionParameters& _params) :
    MotionImpl(_params),
    params_(_params),
    left_(0),
    right_(0)
  {
  }

  void
  DifferentialMotionImpl::getMinMaxLRVelocity(CORBA::Long& minLTranslation,
					      CORBA::Long& maxLTranslation,
					      CORBA::Long& minRTranslation,
					      CORBA::Long& maxRTranslation) throw()
  {
    minLTranslation = params_.minLTranslation;
    maxLTranslation = params_.maxLTranslation;
    minRTranslation = params_.minRTranslation;
    maxRTranslation = params_.maxRTranslation;
  }

  void
  DifferentialMotionImpl::getTargetLRVelocity(CORBA::Long& left,
					      CORBA::Long& right) throw()
  {
    Guard guard(mutex_);

    left = left_;
    right = right_;
  }

  void
  DifferentialMotionImpl::setTargetVelocity(const Miro::VelocityIDL& _velocity) 
  {
    velocity2lr(_velocity, left_, right_);

    // adjust velocties

    if (left_ < params_.minLTranslation) {
      left_ = params_.minLTranslation;
      right_ = (CORBA::Long) (params_.minLTranslation +
			      _velocity.rotation * params_.wheelBase);
    }
    else if (left_ > params_.maxLTranslation) {
      left_ = params_.maxLTranslation;
      right_ = (CORBA::Long) (params_.maxLTranslation +
			      _velocity.rotation * params_.wheelBase);
    }
    if (right_ < params_.minRTranslation) {
      left_ = (CORBA::Long) (params_.minRTranslation -
			     _velocity.rotation * params_.wheelBase);
      right_ = params_.minRTranslation;
    }
    else if (right_ > params_.maxRTranslation) {
      left_ = (CORBA::Long) (params_.maxRTranslation -
			     _velocity.rotation * params_.wheelBase);
      right_ = params_.maxRTranslation;
    }

    // recalculate the resulting velocity
    lr2velocity(left_, right_, targetVelocity_);
  }

  void
  DifferentialMotionImpl::setTargetVelocity(CORBA::Long _left, CORBA::Long _right) 
  {
    left_ = _left;
    right_ = _right;
    lr2velocity(left_, right_, targetVelocity_);
  }

  double
  DifferentialMotionImpl::getWheelBase() throw()
  {
    return params_.wheelBase;
  }
}
