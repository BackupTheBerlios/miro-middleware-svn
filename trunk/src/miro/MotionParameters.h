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
#ifndef MotionParameters_h
#define MotionParameters_h

#include "Parameters.h"

namespace Miro
{
  struct MotionParameters : public Parameters
  {
    long minTranslation;
    long maxTranslation;

    double minRotation;
    double maxRotation;

    MotionParameters();
    void operator <<= (const QDomNode& _node);

  protected:
    void printToStream(std::ostream& _ostr) const;
  };

  struct DifferentialMotionParameters : public MotionParameters
  {
    long minLTranslation;
    long maxLTranslation;

    long minRTranslation;
    long maxRTranslation;

    long wheelBase;

    DifferentialMotionParameters();
    void operator <<= (const QDomNode& _node);

  protected:
    void printToStream(std::ostream& _ostr) const;
  private:
    typedef MotionParameters Super;
  };
};
#endif // velocityParameters_hh
