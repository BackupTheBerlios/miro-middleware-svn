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
  //! Camera parameters.
  /**
   * @author Hans Utz
   */
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

    //! Height of the camera.
    double height;
    //! Angle against the horizon.
    double alpha;

    //! Latency of the video subsystem.
    /** 
     * The time for transferring the image from the real worl
     * into the memory of the process.
     */
    ACE_Time_Value latency;

    CameraParameters();
    virtual void operator <<= (const QDomNode& node);

    //! Singleton instance.
    static Singleton<CameraParameters> instance;
  };

  std::ostream&
  operator << (std::ostream& ostr, const CameraParameters& desc);
};

#endif
 
