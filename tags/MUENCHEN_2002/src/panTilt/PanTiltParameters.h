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
#ifndef PanTiltParameters_h
#define PanTiltParameters_h

#include "miro/TtyParameters.h"
#include "miro/Singleton.h"

namespace DpPanTilt
{
  class Parameters : public Miro::TtyParameters
  {
    typedef Miro::TtyParameters Super;

  public:
    Parameters();
    static Miro::Singleton<Parameters> instance;
  };
};

#endif
