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
#ifndef DifferentialMotionImpl_h
#define DifferentialMotionImpl_h

#include "DifferentialMotionS.h"
#include "MotionImpl.h"
#include "MotionParameters.h"

#include <cmath>

namespace Miro
{
  class DifferentialMotionImpl : public virtual POA_Miro::DifferentialMotion,
				 public MotionImpl
  {
  public:
    DifferentialMotionImpl(const DifferentialMotionParameters& _params);

    virtual void getMinMaxLRVeclocity(CORBA::Long& minLTranslation,
				      CORBA::Long& maxLTranslation,
				      CORBA::Long& minRTranslation,
				      CORBA::Long& maxRTranslation) throw();
    virtual void getTargetLRVelocity(CORBA::Long& left, CORBA::Long& right) throw();

  protected:
    virtual void setTargetVelocity(VelocityIDL& _velocity);
    virtual void setTargetVelocity(CORBA::Long left, CORBA::Long right);

    void testVelocityLRBounds(CORBA::Long left, CORBA::Long right) throw (EOutOfBounds);
    void lr2velocity(CORBA::Long left, CORBA::Long right, VelocityIDL& velocity);
    void velocity2lr(const VelocityIDL& velocity, CORBA::Long& left, CORBA::Long& right);

    const DifferentialMotionParameters& params_;
    CORBA::Long left_;
    CORBA::Long right_;
  };

  inline
  void
  DifferentialMotionImpl::testVelocityLRBounds(CORBA::Long left,
					       CORBA::Long right) throw (EOutOfBounds)
  {
    if (left < params_.minLTranslation ||
	left > params_.maxLTranslation ||
	right < params_.minRTranslation ||
	right > params_.maxRTranslation)
      throw EOutOfBounds();
  }

  inline
  void
  DifferentialMotionImpl::lr2velocity(CORBA::Long left, CORBA::Long right, VelocityIDL& velocity)
  {
    velocity.translation = (left + right) / 2;
    velocity.rotation = (double)(right - left) / (params_.wheelBase * 0.5);
  }

  inline
  void
  DifferentialMotionImpl::velocity2lr(const VelocityIDL& velocity, CORBA::Long& left, CORBA::Long& right)
  {
    left = velocity.translation;
    right = velocity.translation;

    int delta = (int) rint(velocity.rotation * params_.wheelBase * .5);

    left -= delta;
    right += delta;
  }
};

#endif // differentialMotionImpl_hh
