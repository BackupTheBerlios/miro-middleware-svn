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


#include "MotionParameters.h"

#include <qxml.h>

#include <cmath>

namespace Miro
{
  MotionParameters::MotionParameters() :
    minTranslation(-1000),
    maxTranslation( 1000),
    minRotation(-M_PI),
    maxRotation( M_PI)
  {}
   
  void
  MotionParameters::operator <<= (const QDomNode& _node)
  {
  }

  DifferentialMotionParameters::DifferentialMotionParameters() :
    minLTranslation(-1000),
    maxLTranslation( 1000),
    minRTranslation(-1000),
    maxRTranslation( 1000),
    wheelBase(400)
  {}

  void
  DifferentialMotionParameters::operator <<= (const QDomNode& _node)
  {
  }
};
