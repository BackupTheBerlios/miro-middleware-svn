// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BaseParameters_h
#define BaseParameters_h

#include "miro/MotionParameters.h"
#include "miro/Singleton.h"
#include "mcp/McpParameters.h"

namespace Base
{
  class Parameters;
  
  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);

  class Parameters : public Mcp::Parameters
  {
    typedef Mcp::Parameters Super;

  public:
    Miro::MotionParameters motion;

    double  radius;

    int  maxTransVelocity;  // maximum speed (mm / s)
    int  maxTransAccel;     // maximum acceleration (mm / (s*s))
    double  maxRotVelocity; // maximum speed (rad /  s)
    double  maxRotAccel;    // maximum acceleration (rad / (s*s))

    int     voltWarn;
    int     voltPanic;

    Parameters();
    virtual ~Parameters();
    virtual void operator <<= (const QDomNode& node);

    static Miro::Singleton<Parameters> instance;

    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };
};

#endif
