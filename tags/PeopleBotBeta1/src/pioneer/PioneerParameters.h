// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PioneerParameters_h
#define PioneerParameters_h

#include "psos/PsosParameters.h"
#include "miro/MotionParameters.h"
#include "miro/ScanDescriptionC.h"
#include "miro/Singleton.h"

namespace Pioneer
{
  class Parameters;

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);

  class Parameters : public Psos::Parameters
  {
    typedef Psos::Parameters Super;

  public:
    Parameters();
    virtual void operator <<= (const QDomNode& node);

    static Miro::Singleton<Parameters> instance;

    Miro::DifferentialMotionParameters motion;
    Miro::ScanDescriptionIDL sonarDescription;
    Miro::ScanDescriptionIDL tactileDescription;
    Miro::ScanDescriptionIDL infraredDescription;
    bool camera;
    bool cameraUpsideDown;

    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };
};

#endif
