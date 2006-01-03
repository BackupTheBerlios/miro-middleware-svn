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
#ifndef BallDetectionEventChannel_h
#define BallDetectionEventChannel_h

#include "BallDetection.h"               // Filter super class
#include "miro/StructuredPushSupplier.h" // Helper class for event publishing

namespace Video 
{
  //! Detect a red ball
  /**
   * This is just a simple example filter. Better and faster
   * implementations are surely possible.
   */
  class BallDetectionEventChannel : public BallDetection
  {
    typedef BallDetection Super;

  public:
    //! Initializing constructor.
    BallDetectionEventChannel(Miro::ImageFormatIDL const& _inputFormat);

    //! Initialization based on the supplied parameters.
    void init(Miro::Server& _server, Video::FilterParameters const * _params);
    //! Do the work.
    void process();

  private:
    //! Helper class for event publication
    Miro::StructuredPushSupplier * supplier_;
    //! The event to be published
    CosNotification::StructuredEvent event_;
  };
}
#endif
