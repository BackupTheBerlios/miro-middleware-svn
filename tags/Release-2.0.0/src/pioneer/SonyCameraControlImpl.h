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
#ifndef SonyCameraControlImpl_h
#define SonyCameraControlImpl_h

#include "idl/SonyCameraControlS.h"

#include "miro/Exception.h"
#include "miro/CameraControlImpl.h"

#include "pioneer/Parameters.h"

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

  class SonyCameraControlImpl : public virtual POA_Miro::SonyCameraControl,
				public virtual Miro::CameraControlImpl
  {
    typedef Miro::CameraControlImpl Super;

    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    //! Constructor
    SonyCameraControlImpl(Pioneer::Connection& _connection,
			  const Pioneer::Parameters& params);

    //-------------------------------------------------------------------------
    // from CameraControl.idl
    //-------------------------------------------------------------------------
    virtual void setZoom(CORBA::Float value) throw(Miro::EOutOfBounds);
    virtual void setFocus(CORBA::Float value) throw(Miro::EOutOfBounds);
    virtual float getFocus() throw();
    virtual Miro::FocusRangeIDL getFocusRange() throw();
    virtual void setAutoFocus(CORBA::Boolean value) throw(Miro::EDevIO);
    virtual void setShutter(Miro::TimeIDL& value) throw(Miro::EOutOfBounds);

    //-------------------------------------------------------------------------
    // from SonyCameraControl.idl
    //-------------------------------------------------------------------------
    virtual void setAutoExposure(Miro::SonyCameraControl::AutoExposureMode ae)
		 throw(Miro::EOutOfBounds);
    virtual Miro::SonyCameraControl::AutoExposureMode getAutoExposure()
		 throw();
    virtual void setIris(Miro::SonyCameraControl::IrisMode iris)
		 throw(Miro::EOutOfBounds);
    virtual Miro::SonyCameraControl::IrisMode getIris()
		 throw();
    virtual CORBA::Short setGain(CORBA::Short gain)
		 throw(Miro::EOutOfBounds);
    virtual CORBA::Short getGain()
		 throw();
    virtual Miro::GainRangeIDL getGainRange()
		 throw();

  protected:
    Pioneer::Connection& connection; // encapsulating communication to hardware
    //! Video standard: 0 == PAL, 1 == NTSC
    int pal_ntsc;

    //! Current auto-exposure mode
    Miro::SonyCameraControl::AutoExposureMode currentAutoExposure_;
    //! Current iris aperture
    Miro::SonyCameraControl::IrisMode currentIris_;
    //! Current gain setting
    short currentGain_;
    //! Current focal length (depending on the zoom)
    float currentFocalLength_;
    //! Protocol value used during last focus change
    unsigned short lastFocusValue_;

    // Hyperbolic coefficients for zoom->protocol conversion
    double zoom_a;
    double zoom_b;
    double zoom_c;

    // zoom lens limits
    static const float minFocalLength;
    static const float maxFocalLength;

    // low-level protocol values
    static const unsigned short zoom_dev_max;  // tele mode
    static const unsigned short focus_inf;     // infinity
    static const unsigned short focus_near;    // as near as possible

    //! convert a zoom value to low-level representation
    unsigned short zoom2dev(float zooom);
    //! convert a focus value to low-level representation
    unsigned short focus2dev(float focus);
    //! convert a shutter value to a low-level representation
    unsigned char shutter2dev(Miro::TimeIDL& shutter);
  };
}

#endif /* SonyCameraControlImpl_h */
