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
#ifndef PlayerPanTiltImpl_h
#define PlayerPanTiltImpl_h

#include "idl/PanTiltS.h"
#include "miro/PanTiltImpl.h"

#include "miro/Exception.h"

#include <libplayerc++/playerc++.h>
#include <libplayerc++/playerclient.h>

// forward declarations
class ACE_Reactor;

namespace Miro
{
  class Exception;
}

namespace Player
{
  class PlayerConnection;

  class PlayerPanTiltImpl : public virtual POA_Miro::PanTilt , 
			    public virtual Miro::PanTiltImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    PlayerPanTiltImpl(const Miro::PanTiltParameters& _panTiltParameters, bool _upsideDown = false) throw(Miro::Exception);
    virtual ~PlayerPanTiltImpl();

    //-------------------------------------------------------------------------
    // from pan.idl
    //-------------------------------------------------------------------------
    virtual void setPan(CORBA::Float angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual CORBA::Float getPan()
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from tilt.idl
    //-------------------------------------------------------------------------
    virtual void setTilt(CORBA::Float angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual CORBA::Float getTilt()
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // from panTilt.idl
    //-------------------------------------------------------------------------
    virtual Miro::PanTiltPositionIDL getPosition()
      throw(Miro::EDevIO);
    virtual void setPosition(const Miro::PanTiltPositionIDL & dest) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    

    void setPlayerConnection(PlayerConnection * _playerConnection);


  protected:

    PlayerConnection * playerConnection_;
    
    static ACE_Time_Value maxWait;
    double currentPan;
    double currentTilt;
    double panMinSpeed;
    double panMaxSpeed;
    double tiltMinSpeed;
    double tiltMaxSpeed;
    bool upsideDown;

  };
}

#endif




