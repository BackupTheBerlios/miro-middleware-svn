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
#include "AliveCollector.h"

namespace Sparrow
{
  ACE_Time_Value const AliveCollector::TIMEOUT(2, 0);

  AliveCollector::AliveCollector():
    mutex_(),
    lastKickAlive_(0, 0),
    lastPanAlive_(0, 0),
    lastInfrared1Alive_(0, 0),
    lastInfrared2Alive_(0, 0),
    lastMotorAlive_(0, 0)
  {
  }

  bool
  AliveCollector::kickAlive()
  {
    Miro::Guard guard(mutex_);
    return (ACE_OS::gettimeofday() - lastKickAlive_ < TIMEOUT);
  }

  bool
  AliveCollector::panAlive()
  {
    Miro::Guard guard(mutex_);
    return (ACE_OS::gettimeofday() - lastPanAlive_ < TIMEOUT);
  }

  bool
  AliveCollector::infrared1Alive()
  {
    Miro::Guard guard(mutex_);
    return (ACE_OS::gettimeofday() - lastInfrared1Alive_ < TIMEOUT);
  }

  bool
  AliveCollector::infrared2Alive()
  {
    Miro::Guard guard(mutex_);
    return (ACE_OS::gettimeofday() - lastInfrared2Alive_ < TIMEOUT);
  }

  bool
  AliveCollector::motorAlive()
  {
    Miro::Guard guard(mutex_);
    return (ACE_OS::gettimeofday() - lastMotorAlive_ < TIMEOUT);
  }

  void
  AliveCollector::setLastKickAlive(ACE_Time_Value const& _last)
  {
    Miro::Guard guard(mutex_);
    lastKickAlive_ = _last;
  }

  void
  AliveCollector::setLastPanAlive(ACE_Time_Value const& _last)
  {
    Miro::Guard guard(mutex_);
    lastPanAlive_ = _last;
  }

  void
  AliveCollector::setLastInfrared1Alive(ACE_Time_Value const& _last)
  {
    Miro::Guard guard(mutex_);
    lastInfrared1Alive_ = _last;
  }

  void
  AliveCollector::setLastInfrared2Alive(ACE_Time_Value const& _last)
  {
    Miro::Guard guard(mutex_);
    lastInfrared2Alive_ = _last;
  }

  void
  AliveCollector::setLastMotorAlive(ACE_Time_Value const& _last)
  {
    Miro::Guard guard(mutex_);
    lastMotorAlive_ = _last;
  }
}
