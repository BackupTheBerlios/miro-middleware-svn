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
#ifndef PanTiltImpl_h
#define PanTiltImpl_h

#include "idl/PanTiltS.h"
#include "miro/Exception.h"
#include "miro/SvcParameters.h"

#include "miro/PanImpl.h"
#include "miro/TiltImpl.h"

#include <cmath>


namespace Miro
{

  //! Implementation of a dummy PanTilt interface.
  /**
   * This class offers a generic implementation for the PanTilt
   *
   * @author Guillem Pagès Gassull
   */
  class  PanTiltImpl : public virtual POA_Miro::PanTilt,
        public Miro::PanImpl,
        public Miro::TiltImpl
  {
  public:
    //! Initializing constructor.
    PanTiltImpl(PanTiltParameters const& _params);
    virtual ~PanTiltImpl();

    //! PanTilt interface method implementation.
    virtual PanTiltPositionIDL getTargetPosition() throw();
    //! PanTilt interface method implementation.
    virtual PanTiltLimitsIDL getPanTiltLimits() throw();

  protected:
    //------------------------------------------------------------------------
    // protected methods
    //------------------------------------------------------------------------
    bool testPosition(PanTiltPositionIDL const& _value) const throw();
    void setTargetPosition(PanTiltPositionIDL const& _value) throw();

    //------------------------------------------------------------------------
    // protected object data
    //------------------------------------------------------------------------
  };

  inline
  bool
  PanTiltImpl::testPosition(PanTiltPositionIDL const& _value) const throw()
  {
    return (testPan(_value.panValue) && testTilt(_value.tiltValue));
  }

  inline
  void
  PanTiltImpl::setTargetPosition(PanTiltPositionIDL const& _value) throw()
  {
    setTargetPan(_value.panValue);
    setTargetTilt(_value.tiltValue);
  }
}

#endif // PanTiltImpl_h
