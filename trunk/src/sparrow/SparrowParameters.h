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
#ifndef SparrowParameters_h
#define SparrowParameters_h

#include "can/CanParameters.h"

#include "miro/MotionParameters.h"
#include "miro/ScanDescriptionC.h"
#include "miro/Singleton.h"
#include <ace/OS.h>

#include <iostream>
#include <vector>

namespace Sparrow
{
  struct InfraredScaling
  {
    int offset;
    double scaling;
    int minDistance;
    int maxDistance;

    InfraredScaling();
  };

  std::ostream& operator << (std::ostream& ostr, const InfraredScaling& rhs);
  std::istream& operator >> (std::istream& istr, InfraredScaling& rhs);

  class Parameters;
  
  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);

  class Parameters : public Can::Parameters
  {
    typedef Can::Parameters Super;
  public:
    Miro::DifferentialMotionParameters motion;

    bool goalie;

    int ticksL;
    int ticksR;
    int diameter;

    int    maxAccel;
    int    maxPower;
    int    maxTransVelocity;
    double maxRotVelocity;

    int stallThreshold;
    int stallA2;
    int stallA1;
    int stallA0;

    int initialX;
    int initialY;
    double initialHeading;

    ACE_Time_Value watchdogTimeout;
    ACE_Time_Value buttonsPollInterval;

    unsigned short servo0MidPulse;
    unsigned short servo1MidPulse;
    double deg2servoTicks;
    int panLatency;
    int panMSecPerRad;
    int panSwing;

    double transFactor;

    bool writeTables;
    std::vector<short> table1;
    std::vector<short> table2;

    std::vector<InfraredScaling> irScaling;

    int infraredInterval;
    int odometryInterval;

    Miro::ScanDescriptionIDL infraredDescription;

    bool channelSharing;

    Parameters();
    virtual void operator <<= (const QDomNode& node);

    static Miro::Singleton<Parameters> instance;

    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };
};

#endif
