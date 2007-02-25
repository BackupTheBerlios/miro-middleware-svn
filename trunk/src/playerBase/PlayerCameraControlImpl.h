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
#ifndef PlayerCameraControlImpl_h
#define PlayerCameraControlImpl_h

#include "miro/CameraControlImpl.h"

#include "miro/Exception.h"
#include "miro/SvcParameters.h"
#include "miro/TimeHelper.h"
#include <cmath>
#include <playerc++.h>
#include <playerclient.h>

class PlayerConnection;

namespace Player
{

  //! Implementation of a Player CameraControl interface.
  /**
   * This class offers an implementation for the CameraControl interface
   *  for Player.
   * 
   * It only implements zoom functionality. The other interface functions
   *  are inherited from the default 
   *
   * @author Guillem Pagès Gassull
   */
  class  CameraControlImpl : public virtual POA_Miro::CameraControl,
			     public virtual Miro::CameraControlImpl
  {
  public:
    typedef Miro::CameraControlImpl Super;

    //! Initializing constructor.
    CameraControlImpl(const Miro::CameraControlParameters& _cameraParameters);
    virtual ~CameraControlImpl();


    //! CameraControl interface method implementation.
    virtual void setZoom(float value) throw(Miro::EDevIO, Miro::EOutOfBounds,Miro::ETimeOut);
    //! CameraControl interface method implementation.
    virtual float getZoom() throw(Miro::EDevIO, Miro::ETimeOut);


    void setPlayerConnection(PlayerConnection * _playerConnection);

  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    
    PlayerConnection * playerConnection_;

  };

}

#endif // PlayerCameraControlImpl_h
