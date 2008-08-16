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
#ifndef PlayerMotionImpl_h
#define PlayerMotionImpl_h

#include "miro/MotionImpl.h"
#include "idl/MotionS.h"
#include <libplayerc++/playerc++.h>
#include <libplayerc++/playerclient.h>

class PositionProxy;

using namespace PlayerCc;

namespace Miro
{
 
  //! Simulator implementation for the generalized Motion interface.
  /**
   * This class provides support for the set method
   * of the generalized Motion interface for the Player simulator
   */
  class PlayerMotionImpl : public virtual POA_Miro::Motion, Miro::MotionImpl
  {
    typedef Miro::MotionImpl Super;
  public:
    //! Initializing constructor.
    PlayerMotionImpl(const MotionParameters& _params);

    void setPlayerPositionProxy(Position2dProxy* _playerPosition);

    virtual void limp() throw(EDevIO);

    virtual void setVelocity(const VelocityIDL& velocity) throw(EOutOfBounds, EDevIO);

  protected:
    Position2dProxy* playerPositionProxy;

  };

};

#endif // PlayerMotionImpl_h
