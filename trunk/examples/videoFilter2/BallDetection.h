// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BallDetection_h
#define BallDetection_h

#include "ParametersFilter.h"   // Automatically generated parameter class
#include "ParametersImage.h"    // Automatically generated output parameters
#include "video/VideoFilter.h"  // Filter base class

namespace Video
{
  //! Detect a red ball
  /**
   * This is just a simple example filter. Better and faster
   * implementations are surely possible.
   */
  class BallDetection : public Video::Filter
  {
    typedef Video::Filter Super;
  public:
    //! Initializing constructor.
    BallDetection(Miro::ImageFormatIDL const& _inputFormat);
    //! Do the work.
    void process();

    //! Factory method for parameter class.
    FILTER_PARAMETERS_FACTORY(BallDetection);
    IMAGE_PARAMETERS_FACTORY(BallFeature);
  };
}
#endif
