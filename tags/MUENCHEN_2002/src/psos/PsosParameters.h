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
#ifndef PsosParameters_h
#define PsosParameters_h

#include "miro/TtyParameters.h"

namespace Psos
{
  class Parameters;

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);

  class Parameters : public Miro::TtyParameters
  {
    typedef Miro::TtyParameters Super;

  public:

    // Conversion factors
    double angleConvFactor; // radians per angular unit (2PI/4096)
    double distConvFactor;  // mm returned by P2
    double velConvFactor;   // mm/sec returned by P2
    double rangeConvFactor; // sonar range returned in mm
    double diffConvFactor;  // ratio of angular velocity to wheel velocity difference
    double vel2Divisor;     // divisor for sending VEL2 commands to the robot

    Parameters();
    void operator <<= (const QDomNode& node);
    
    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };

};

#endif
