// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef ConstraintArbiterParameters_hh
#define ConstraintArbiterParameters_hh

#include "Parameters.h"
#include "VelocitySpace.h"

#include <ace/Time_Value.h>

namespace Miro
{
  class ConstraintArbiterParameters : public PriorityArbiterParameters
  {
    typedef PriorityArbiterParameters Super;

  public: 
    ACE_Time_Value pace;
    int maxAccel;
    int maxDecel;
    int spaceResolution;
    int maxVelocity;
    bool viewerTask;

    VelocitySpace velocitySpace;

    ConstraintArbiterParameters();

    virtual void operator <<= (const QDomNode&);
    virtual QDomElement operator >>= (QDomNode&) const;
  protected:
    virtual void printToStream(std::ostream&) const;
  };
}
#endif // ConstraintArbiterParameters_hh
