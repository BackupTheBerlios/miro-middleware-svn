// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef CameraParameters_h
#define CameraParameters_h

#include "Parameters.h"
#include "Singleton.h"

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

    CameraParameters();
    virtual void operator <<= (const QDomNode& node);

    static Singleton<CameraParameters> instance;
  };

  std::ostream&
  operator << (std::ostream& ostr, const CameraParameters& desc);
};

#endif
 
