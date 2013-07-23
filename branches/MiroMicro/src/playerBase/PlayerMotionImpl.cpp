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
#include "PlayerMotionImpl.h"

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

  void PlayerMotionImpl::setPlayerPositionProxy(Position2dProxy* _playerPosition) 
  {

    playerPositionProxy=_playerPosition;

    if (playerPositionProxy) {
      playerPositionProxy->SetMotorEnable(1);
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
