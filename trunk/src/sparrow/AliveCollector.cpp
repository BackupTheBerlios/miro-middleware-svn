// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
