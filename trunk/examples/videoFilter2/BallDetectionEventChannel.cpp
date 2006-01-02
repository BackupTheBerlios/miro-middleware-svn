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

#include "BallDetectionEventChannel.h"
#include "BallResultC.h" 

#include "miro/VideoHelper.h" // Global helper functions.
#include "miro/Exception.h"   // Exception declarations.

#include "miro/Server.h"

namespace Video {

  // Factory method implementation.
  FILTER_PARAMETERS_FACTORY_IMPL(BallDetectionEventChannel);
  IMAGE_PARAMETERS_FACTORY_IMPL_2(BallFeature, BallDetectionEventChannel);

  /**
   * The filter expects the input format palette to be Miro::RGB_24. Otherwise
   * a Miro::Exception is thrown.
   */
  BallDetectionEventChannel::BallDetectionEventChannel(Miro::ImageFormatIDL const& _inputFormat) :
    Video::Filter(_inputFormat)
  {
    if (inputFormat_.palette != Miro::RGB_24)
      throw Miro::Exception("BallDetectionEventChannel: unsupported input format (RGB required).");
    outputFormat_.palette = Miro::GREY_8;
  }


  /**
   * Processing of the configuration file parameters.
   */
  void
  BallDetectionEventChannel::init(Miro::Server& _server, Video::FilterParameters const * _params)
  {
    // Down cast the params pointer to the BallDetectionEventChannel parameters class.
    params_ = dynamic_cast<BallDetectionEventChannelParameters const *>(_params);
    // Assert that the cast was successfull.
    assert(params_ != NULL);

    // Super class initialization.
    Video::Filter::init(_server, _params);

    // Set up the interface
    try {
      // Get reference to the event channel
      CosNotifyChannelAdmin::EventChannel_var ec =
	_server.resolveName<CosNotifyChannelAdmin::EventChannel>("EventChannel");
      supplier_ = new Miro::StructuredPushSupplier(ec.in(), _server.namingContextName.c_str());

      // Define the event type which is published
      event_.header.fixed_header.event_type.domain_name =
	CORBA::string_dup(supplier_->domainName().c_str());
      event_.header.fixed_header.event_type.type_name =
	CORBA::string_dup("BallResult");
      event_.header.fixed_header.event_name = CORBA::string_dup("");
      event_.header.variable_header.length(0);
      event_.filterable_data.length(0);

      // Register the event
      CosNotification::EventTypeSeq types;
      types.length(1);
      types[0] = event_.header.fixed_header.event_type;
      supplier_->addOffers(types);
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
    // Pointer to the input image.
    unsigned char const * src = inputBuffer();
    // Pointer to the output image.
    unsigned char * dest = outputBuffer();
    Video::FilterImageParameters  * imageParams = outputBufferParameters();
    assert(imageParams != NULL);
    BallFeatureImageParameters * output = dynamic_cast<BallFeatureImageParameters *>(imageParams);
    assert(output != NULL);

    int sum = 0;
    long sumX = 0;
    long sumY = 0;
    for (unsigned int x=0; x<inputFormat_.width; ++x)
      for (unsigned int y=0; y<inputFormat_.height; ++y) {
	if ((src[y*inputFormat_.width*3+x*3] <= params_->redHigh) && 
	    (src[y*inputFormat_.width*3+x*3] >= params_->redLow) &&
	    (src[y*inputFormat_.width*3+x*3+1] <= params_->greenHigh) && 
	    (src[y*inputFormat_.width*3+x*3+1] >= params_->greenLow) &&
	    (src[y*inputFormat_.width*3+x*3+2] <= params_->blueHigh) && 
	    (src[y*inputFormat_.width*3+x*3+2] >= params_->blueLow)) {
	  sum++;
	  sumX += x;
	  sumY += y;
	  dest[y*inputFormat_.width+x] = 255;
	} else {
	  dest[y*inputFormat_.width+x] = 0;
	}
      }
    if ((double)sum/(double)(inputFormat_.width*inputFormat_.height) > params_->redRatio) {
      output->x = (int)((double)sumX/(double)sum);
      output->y = (int)((double)sumY/(double)sum);

      // Send the found ball position
      BallResultIDL * result = new BallResultIDL();
      result->x = output->x;
      result->y = output->y;
      event_.remainder_of_body <<= result;
      supplier_->sendEvent(event_);
    }
  }
};
