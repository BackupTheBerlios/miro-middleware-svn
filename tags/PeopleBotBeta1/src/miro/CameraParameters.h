// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef CameraParameters_h
#define CameraParameters_h

#include "Parameters.h"
#include "Singleton.h"

#include <ace/OS.h>

namespace Miro
{
  class CameraParameters : public Parameters
  {
    typedef Parameters Super;

  public:
    double ncx;
    double nfx;
    double dx;
    double dy;

    double cx;
    double cy;
    double sx;
    double f;
    double kappa;

    double height;
    double alpha;

    ACE_Time_Value latency;

    CameraParameters();
    virtual void operator <<= (const QDomNode& node);

    static Singleton<CameraParameters> instance;
  };

  std::ostream&
  operator << (std::ostream& ostr, const CameraParameters& desc);
};

#endif
 
