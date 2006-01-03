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

#include "BallDetectionEventChannel.h"
#include "BallResultC.h" 

#include "miro/VideoHelper.h" // Global helper functions.
#include "miro/Exception.h"   // Exception declarations.
#include "miro/Server.h"

namespace {
  using CosNotifyChannelAdmin::EventChannel;
  using CosNotifyChannelAdmin::EventChannel_var;
}

namespace Video 
{
  /**
   * The filter expects the input format palette to be Miro::RGB_24. Otherwise
   * a Miro::Exception is thrown.
   */
  BallDetectionEventChannel::BallDetectionEventChannel(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {}

  /**
   * Processing of the configuration file parameters.
   */
  void
  BallDetectionEventChannel::init(Miro::Server& _server, Video::FilterParameters const * _params)
  {
    // Super class initialization.
    Super::init(_server, _params);

    // Set up the interface
    try {
      // Get reference to the event channel
      CosNotifyChannelAdmin::EventChannel_var ec =
	_server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel");
      supplier_ = new Miro::StructuredPushSupplier(ec.in(), _server.namingContextName.c_str());

      // Define the event type which is published
      Miro::StructuredPushSupplier::
	initStructuredEvent(event_, supplier_->domainName(), "BallResult");

      // Register the event at the event channel
      supplier_->addOffers(event_);
    }
    catch (const CORBA::Exception& e) {
      std::cerr << "Set up event publication failed: " << e << std::endl;
    }
  }


  /**
   * Ball detection.
   */
  void
  BallDetectionEventChannel::process()
  {
    // Pointer to the output meta data
    Video::FilterImageParameters  * imageParams = outputBufferParameters();
    ImageParameters * output =
      dynamic_cast<ImageParameters *>(imageParams);

    Super::process();

    // Send the found ball position
    BallResult result;
    result.x = output->x;
    result.y = output->y;
    event_.remainder_of_body <<= result;
    supplier_->sendEvent(event_);
  }
}
