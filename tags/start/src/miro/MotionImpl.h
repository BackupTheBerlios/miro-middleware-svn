// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroMotionImpl_hh
#define miroMotionImpl_hh

#include "MotionS.h"
#include "MotionParameters.h"

#include "Synch.h"

namespace Miro
{
  class MotionImpl : public virtual POA_Miro::Motion
  {
  public:
    MotionImpl(const MotionParameters& _params);

    virtual void getMinMaxVeclocity(CORBA::Long& minTranslation,
				    CORBA::Long& maxTranslation,
				    double& minRotation,
				    double& maxRotation) throw();
    virtual VelocityIDL getTargetVelocity() throw();

  protected:
    virtual void setTargetVelocity(VelocityIDL& _velocity);
    void testVelocityBounds(const VelocityIDL& _velocity) throw (EOutOfBounds);

    const MotionParameters& params_;
    Mutex mutex_;
    VelocityIDL targetVelocity_;
  };

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

#endif // motionImpl_hh
