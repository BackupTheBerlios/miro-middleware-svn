// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
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
//
// Authors: 
//   Ingo van Lil
//
#ifndef SonyPanTiltImpl_h
#define SonyPanTiltImpl_h

#include "idl/PanTiltS.h"

#include "miro/Exception.h"
#include "miro/PanTiltImpl.h"

// forward declarations
namespace Psos
{
  class Message;
}

namespace Pioneer
{
  class Connection;
}

namespace Sony
{
  class Message;

  class SonyPanTiltImpl : public virtual Miro::PanTiltImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    //! Constructor
    SonyPanTiltImpl(Pioneer::Connection& _connection,
		    const Miro::PanTiltParameters& _params,
		    bool _upsideDown = false);

    //-------------------------------------------------------------------------
    // from Pan.idl
    //-------------------------------------------------------------------------
    virtual void setPan(CORBA::Float value) throw(Miro::EOutOfBounds);
    virtual CORBA::Float getPan() throw();

    //-------------------------------------------------------------------------
    // from Tilt.idl
    //-------------------------------------------------------------------------
    virtual void setTilt(CORBA::Float angle) throw(Miro::EOutOfBounds);
    virtual CORBA::Float getTilt() throw();

    //-------------------------------------------------------------------------
    // from PanTilt.idl
    //-------------------------------------------------------------------------
    virtual void setPosition(const Miro::PanTiltPositionIDL& destination)
      throw(Miro::EOutOfBounds);
    virtual Miro::PanTiltPositionIDL getPosition() throw();

  protected:
    Pioneer::Connection& connection; // encapsulating communication to hardware
    const Miro::PanTiltParameters& params;
    bool upsideDown;

    static const unsigned short pan_maxdev = 0x0370; // -0x0370 .. 0x0370
    static const unsigned short tilt_maxdev = 0x012c; // -0x012c .. 0x012c

    //! convert a pan angle to low-level representation
    unsigned short pan2dev(float pan);
    //! convert a tilt angle to low-level representation
    unsigned short tilt2dev(float tilt);
  };
}

#endif /* SonyPanTiltImpl_h */

