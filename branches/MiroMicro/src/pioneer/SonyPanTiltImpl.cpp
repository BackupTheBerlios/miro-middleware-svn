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
#include "pioneer/SonyPanTiltImpl.h"
#include "pioneer/SonyMessage.h"
#include "pioneer/PioneerConnection.h"
#include "psos/PsosMessage.h"

#include "miro/Exception.h"
#include "miro/Angle.h"
#include "miro/Log.h"
#include "miro/Synch.h"

#include <iostream>

namespace Sony
{
  SonyPanTiltImpl::SonyPanTiltImpl(Pioneer::Connection& _connection,
				   const Miro::PanTiltParameters& _params,
				   bool _upsideDown) :
    PanTiltImpl(_params),
    connection(_connection),
    params(_params),
    upsideDown(_upsideDown)
  {
    MIRO_LOG_CTOR("Sony::SonyPanTiltImpl");
    MIRO_LOG_CTOR_END("Sony::SonyPanTiltImpl");
  }

  // IDL interface implementation:
  //-------------------------------------------------------------------------
  // from Pan.idl
  //-------------------------------------------------------------------------
  void
  SonyPanTiltImpl::setPan(CORBA::Float angle) throw(Miro::EOutOfBounds)
  {
    if (! testPan(angle))
      throw Miro::EOutOfBounds();

    targetPan_ = angle;
    Sony::Message msg;
    msg.panTilt(pan2dev(targetPan_), tilt2dev(targetTilt_));
    connection.sendCamera(msg);
  }

  CORBA::Float
  SonyPanTiltImpl::getPan() throw()
  {
    return targetPan_;
  }

  //-------------------------------------------------------------------------
  // from Tilt.idl
  //-------------------------------------------------------------------------
  void
  SonyPanTiltImpl::setTilt(CORBA::Float angle) throw(Miro::EOutOfBounds)
  {
    if (! testTilt(angle))
      throw Miro::EOutOfBounds();

    targetTilt_ = angle;
    Sony::Message msg;
    msg.panTilt(pan2dev(targetPan_), tilt2dev(targetTilt_));
    connection.sendCamera(msg);
  }

  CORBA::Float
  SonyPanTiltImpl::getTilt() throw()
  {
    return targetTilt_;
  }

  //-------------------------------------------------------------------------
  // from PanTilt.idl
  //-------------------------------------------------------------------------
  void
  SonyPanTiltImpl::setPosition(const Miro::PanTiltPositionIDL& dest)
	throw(Miro::EOutOfBounds)
  {
    if (! testPosition(dest))
      throw Miro::EOutOfBounds();

    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(mutex_);
    targetPan_ = dest.panValue;
    targetTilt_ = dest.tiltValue;
    Sony::Message msg;
    msg.panTilt(pan2dev(targetPan_), tilt2dev(targetTilt_));
    connection.sendCamera(msg);
  }

  Miro::PanTiltPositionIDL
  SonyPanTiltImpl::getPosition() throw()
  {
    return getTargetPosition();
  }

  // helper functions:
  unsigned short SonyPanTiltImpl::pan2dev(float pan)
  {
    unsigned short p =
		(unsigned short)((pan / params.pan.rangeMax) * pan_maxdev);
    return (upsideDown? p : -p); // camera angles are specified clockwise
  }

  unsigned short SonyPanTiltImpl::tilt2dev(float tilt)
  {
    unsigned short t =
		(unsigned short)((tilt / params.tilt.rangeMax) * tilt_maxdev);
    return (upsideDown? -t : t);
  }
}
