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

#ifndef BallDetectionEventChannel_h
#define BallDetectionEventChannel_h

#include "ParametersFilter.h"         // Automatically generated parameter class
#include "ParametersImage.h"         // Automatically generated output parameters
#include "video/VideoFilter.h"  // Filter base class
#include "miro/StructuredPushSupplier.h" // Helper class for event publishing


namespace Video {

  //! Detect a red ball
  /**
   * This is just a simple example filter. Better and faster
   * implementations are surely possible.
   */
  class BallDetectionEventChannel : public Video::Filter
  {
  public:
    //! Initializing constructor.
    BallDetectionEventChannel(Miro::ImageFormatIDL const& _inputFormat);

    //! Factory method for parameter class.
    FILTER_PARAMETERS_FACTORY(BallDetectionEventChannel);
    IMAGE_PARAMETERS_FACTORY(BallFeature);

    //! Initialization based on the supplied parameters.
    void init(Miro::Server& _server, Video::FilterParameters const * _params);
    //! Do the work.
    void process();

  private:
    //! Pointer to the instances parameters.
    BallDetectionEventChannelParameters const * params_;
    //! Helper class for event publication
    Miro::StructuredPushSupplier * supplier_;
    //! The event to be published
    CosNotification::StructuredEvent event_;
  };
};

#endif
