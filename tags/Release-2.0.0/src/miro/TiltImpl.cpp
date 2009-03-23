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
#include "TiltImpl.h"
#include "Angle.h"

namespace Miro
{

  TiltImpl::TiltImpl(TiltParameters const& _params) :
    params_(_params),
    targetTilt_(0)
  {}

  TiltImpl::~TiltImpl() 
  {}

  CORBA::Float TiltImpl::getTargetTilt() throw() 
  {
    return targetTilt_;
  }

  TiltLimitsIDL TiltImpl::getTiltLimits() throw()
  {
    TiltLimitsIDL result;
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);

    result.minAngle = params_.rangeMin;
    result.maxAngle = params_.rangeMax;

    return result;
  }
}
