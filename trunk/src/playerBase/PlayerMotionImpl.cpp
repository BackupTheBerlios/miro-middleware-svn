// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PlayerMotionImpl.h"

#include <playerclient.h>

namespace Miro
{
  /**
   * @param _params Reference to the MotionParameters for the
   * motion device.
   */
  PlayerMotionImpl::PlayerMotionImpl(const MotionParameters& _params) :
    Super(_params),
    playerPositionProxy(NULL)
  {
  }

  void PlayerMotionImpl::setPlayerPositionProxy(PositionProxy* _playerPosition) 
  {

    playerPositionProxy=_playerPosition;

    if (playerPositionProxy) {
      playerPositionProxy->SetMotorState(1);
    }
  }
  
  void PlayerMotionImpl::limp() throw(EDevIO)
  {
    if (playerPositionProxy != NULL) {
      playerPositionProxy->SetSpeed(0,0,0);
    }

  }

  void
  PlayerMotionImpl::setVelocity(const VelocityIDL& _velocity) throw(EOutOfBounds, EDevIO)
  {
    Guard guard(mutex_);

    try {
      testVelocityBounds(_velocity);
    }
    catch (EOutOfBounds& e) {
      std::cout << "OutOfBounds" << std::endl;
      throw e;
    }
    setTargetVelocity(_velocity);
    if (playerPositionProxy) {
      playerPositionProxy->SetSpeed(double(_velocity.translation)/1000.0,_velocity.rotation);
   }
  }

};
