// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroMotionImpl_h
#define miroMotionImpl_h

#include "MotionS.h"
#include "MotionParameters.h"
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
	_velocity.rotation > params_.maxRotation)
      throw EOutOfBounds();
  }
};

#endif // MotionImpl_h
