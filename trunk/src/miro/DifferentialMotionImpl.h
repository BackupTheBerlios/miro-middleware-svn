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
#ifndef DifferentialMotionImpl_h
#define DifferentialMotionImpl_h

#include "idl/DifferentialMotionS.h"
#include "MotionImpl.h"
#include "miro/SvcParameters.h"

#include <cmath>

namespace Miro
{
  //! Base implementation for the DifferentialMotion interface.
  /**
   * This class provides generic support for the query
   * methods of the DifferetialMotion interface and can
   * be used as a mix in class for a robots specialized
   * DifferentialMotion interface implementation.
   *
   * Note that it inherits also the base implementation for the 
   * generalized Motion interface.
   */
  class DifferentialMotionImpl : public virtual POA_Miro::DifferentialMotion,
				 public MotionImpl
  {
  public:
    //! Initializing constructor.
    DifferentialMotionImpl(const DifferentialMotionParameters& _params);

    //! DifferentialMotion interface method implementation.
    virtual void getMinMaxLRVelocity(CORBA::Long& minLTranslation,
				     CORBA::Long& maxLTranslation,
				     CORBA::Long& minRTranslation,
				     CORBA::Long& maxRTranslation) throw();
    //! DifferentialMotion interface method implementation.
    virtual void getTargetLRVelocity(CORBA::Long& left, CORBA::Long& right) throw();
    //! Report the wheelbase.
    virtual double getWheelBase() throw();

    //! Convert per wheel velocities in translation/rotation.
    void lr2velocity(CORBA::Long left, CORBA::Long right, VelocityIDL& velocity) throw();
    //! Convert translation/rotation into per wheel velocities.
    void velocity2lr(const VelocityIDL& _velocity, CORBA::Long& _left, CORBA::Long& _right) throw();

  protected:
    //! Memorize target velocity.
    virtual void setTargetVelocity(const VelocityIDL& _velocity);
    //! Memorize target velocity.
    virtual void setTargetVelocity(CORBA::Long _left, CORBA::Long _right);

    //! Generalized test for wheel velocity parameter correctness.
    void testVelocityLRBounds(CORBA::Long _left, CORBA::Long _right) throw (EOutOfBounds);

    //! Reference to DifferentialMotion parameters
    const DifferentialMotionParameters& params_;
    //! Current target velocity of left wheel.
    CORBA::Long left_;
    //! Current target velocity of right wheel.
    CORBA::Long right_;
  };

  //--------------------------------------------------------------------------
  // inlines
  //--------------------------------------------------------------------------

  /**
   * @param _left    Velocity to test for left whell.
   * @param _right   Velocity to test for right whell.
   * @throw EOutOfBounds if a per wheel velocity excedes the bounds 
   * specified in @ref params_.
   */
  inline
  void
  DifferentialMotionImpl::testVelocityLRBounds(CORBA::Long _left,
					       CORBA::Long _right) throw (EOutOfBounds)
  {
    if (_left < params_.minLTranslation ||
	_left > params_.maxLTranslation ||
	_right < params_.minRTranslation ||
	_right > params_.maxRTranslation) {
      char buffer[50];
      sprintf(buffer, 
	      "Left wheel: %d\nRight wheel: %d", _left, _right);
      throw EOutOfBounds(buffer);
    }
  }

  inline
  void
  DifferentialMotionImpl::lr2velocity(CORBA::Long left, CORBA::Long right,
				      VelocityIDL& velocity)
    throw()
  {
    velocity.translation = (left + right) / 2;
    velocity.rotation = (double)(right - left) / (params_.wheelBase);
  }

  inline
  void
  DifferentialMotionImpl::velocity2lr(const VelocityIDL& velocity, 
				      CORBA::Long& left, CORBA::Long& right)
    throw()
  {
    left = velocity.translation;
    right = velocity.translation;

    int delta = (int) rint(velocity.rotation * params_.wheelBase * .5);
    
    //if  left speed > params_.maxLTranslation because of rotation we need to speed down
    if (left-delta > params_.maxLTranslation) {
	    left = params_.maxLTranslation;
	    right = right + 2*delta;
    }else{
	     // here the same for right
	    if (right+delta > params_.maxRTranslation) {
		    right = params_.maxRTranslation;
		    left  = left - 2*delta;
	    }else{
		    left -= delta;
		    right += delta;
           }
    }
  }
}

#endif // DifferentialMotionImpl_h
