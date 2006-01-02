//////////////////////////////////////////////////////////////////////////////
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

#include "ParametersFilter.h"         // Automatically generated parameter class
#include "ParametersImage.h"         // Automatically generated output parameters
#include "video/VideoFilter.h"  // Filter base class


namespace Video {

  //! Detect a red ball
  /**
   * This is just a simple example filter. Better and faster
   * implementations are surely possible.
   */
  class BallDetection : public Video::Filter
  {
  public:
    //! Initializing constructor.
    BallDetection(Miro::ImageFormatIDL const& _inputFormat);

    //! Factory method for parameter class.
    FILTER_PARAMETERS_FACTORY(BallDetection);
    IMAGE_PARAMETERS_FACTORY(BallFeature);

    //! Initialization based on the supplied parameters.
    void init(Miro::Server& _server, Video::FilterParameters const * _params);
    //! Do the work.
    void process();

  private:
    //! Pointer to the instances parameters.
    BallDetectionParameters const * params_;
  };
};

#endif
