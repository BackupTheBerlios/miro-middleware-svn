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
#ifndef TiltImpl_h
#define TiltImpl_h

#include "PanTiltBase.h"
#include "idl/TiltS.h"
#include "miro/SvcParameters.h"

namespace Miro
{

  //! Partial implementation of the Tilt interface.
  /**
   * This class offers a generic part of a tilt implementation.
   *
   * @author Guillem Pagès Gassull
   */
  class  TiltImpl : public virtual PanTiltBase,
        public virtual POA_Miro::Tilt
  {
  public:
    //! Initializing constructor.
    TiltImpl(Miro::TiltParameters const& _params);
    virtual ~TiltImpl();

    //! Tilt interface method implementation.
    virtual float getTargetTilt() throw();
    //! Tilt interface method implementation.
    virtual TiltLimitsIDL getTiltLimits() throw();

  protected:
    //-------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------

    //! Test if tilt angle is within the limits.
    bool testTilt(CORBA::Float _angle) const throw();
    //! Set the new target pan angle.
    void setTargetTilt(CORBA::Float _angle) throw();

    //-------------------------------------------------------------------------
    // protected data
    //-------------------------------------------------------------------------
    TiltParameters const& params_;
    CORBA::Float targetTilt_;
  };

  inline
  bool
  TiltImpl::testTilt(CORBA::Float _angle) const throw()
  {
    return ((_angle >= float(params_.rangeMin)) &&
            (_angle <= float(params_.rangeMax)));
  }

  inline
  void
  TiltImpl::setTargetTilt(CORBA::Float _angle) throw()
  {
    targetTilt_ = _angle;
  }
}

#endif // TiltImpl_h
