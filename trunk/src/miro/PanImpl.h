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
#ifndef PanImpl_h
#define PanImpl_h

#include "PanTiltBase.h"
#include "idl/PanS.h"
#include "miro/SvcParameters.h"

namespace Miro
{

  //! Implementation of a base Pan interface.
  /**
   * This class offers a generic implementation for the Pan
   *
   * @author Guillem Pagès Gassull
   */
  class  PanImpl : public virtual PanTiltBase,
		   public virtual POA_Miro::Pan
  {
  public:
    //! Initializing constructor.
    PanImpl(PanParameters const& _params);
    virtual ~PanImpl();
   

    //! Pan interface method implementation.
    virtual CORBA::Float getTargetPan() throw();
    //! Pan interface method implementation.
    virtual PanLimitsIDL getPanLimits() throw();

  protected:
    //-------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------

    //! Test if pan angle is within the limits.
    bool testPan(CORBA::Float _angle) const throw();
    //! Set the new target pan angle.
    void setTargetPan(CORBA::Float _angle) throw();

    //-------------------------------------------------------------------------
    // protected data
    //-------------------------------------------------------------------------
    PanParameters const& params_;
    double targetPan_;
  };

  inline 
  bool 
  PanImpl::testPan(CORBA::Float _angle) const throw() {
    return ((_angle >= float(params_.rangeMin)) &&
	    (_angle <= float(params_.rangeMax)));
  }

  inline 
  void 
  PanImpl::setTargetPan(CORBA::Float _angle) throw() {
    targetPan_ = _angle; 
  }
}

#endif // PanImpl_h
