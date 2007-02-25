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
#ifndef miroMotionImpl_h
#define miroMotionImpl_h

#include "idl/MotionS.h"
#include "miro/SvcParameters.h"
#include "Synch.h"

namespace Miro
{
  //! Base implementation for the generalized Motion interface.
  /**
   * This class provides generic support for the query
   * methods of the generalized Motion interface and can
   * be used as a mix in class for a robots specialized
   * Motion interface implementation.
   */
  class MotionImpl : public virtual POA_Miro::Motion
  {
  public:
    //! Initializing constructor.
    MotionImpl(const MotionParameters& _params);

    //! Motion interface method implementation.
    virtual void getMinMaxVelocity(CORBA::Long& minTranslation,
				   CORBA::Long& maxTranslation,
				   double& minRotation,
				   double& maxRotation) throw();
    //! Motion interface method implementation.
    virtual VelocityIDL getTargetVelocity() throw();

  protected:
    //! Memorize target velocity
    virtual void setTargetVelocity(const VelocityIDL& _velocity);
    //! Generalized test for velocity parameter correctness.
    void testVelocityBounds(const VelocityIDL& _velocity) throw (EOutOfBounds);

    //! Reference to Motion parameters
    const MotionParameters& params_;
    //! Central Motion lock.
    Mutex mutex_;
    //! Current target velocity.
    VelocityIDL targetVelocity_;
  };

  //--------------------------------------------------------------------------
  // inlines
  //--------------------------------------------------------------------------

  /**
   * @param _velocity    Velocity to test.
   * @throw EOutOfBounds if @reg _velocity excedes the bounds 
   * specified in @ref params_.
   */
  inline
  void
  MotionImpl::testVelocityBounds(const VelocityIDL& _velocity) throw (EOutOfBounds)
  {
    if (_velocity.translation < params_.minTranslation ||
	_velocity.translation > params_.maxTranslation ||
	_velocity.rotation < params_.minRotation ||
	_velocity.rotation > params_.maxRotation) {
      char buffer[50];
      sprintf(buffer, 
	      "Translation: %d\nRotation: %f", 
	      _velocity.translation, 
	      Miro::rad2Deg(_velocity.rotation));
      throw EOutOfBounds(buffer);
    }
  }
};

#endif // MotionImpl_h
