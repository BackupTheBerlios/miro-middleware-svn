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

#include "BallDetection.h"
#include "miro/VideoHelper.h" // Global helper functions.
#include "miro/Exception.h"   // Exception declarations.


namespace Video {

  // Factory method implementation.
  FILTER_PARAMETERS_FACTORY_IMPL(BallDetection);
  IMAGE_PARAMETERS_FACTORY_IMPL_2(BallFeature, BallDetection);


  /**
   * The filter expects the input format palette to be Miro::RGB_24. Otherwise
   * a Miro::Exception is thrown.
   */
  BallDetection::BallDetection(Miro::ImageFormatIDL const& _inputFormat) :
    Video::Filter(_inputFormat)
  {
    if (inputFormat_.palette != Miro::RGB_24)
      throw Miro::Exception("BallDetection: unsupported input format (RGB required).");
    outputFormat_.palette = Miro::GREY_8;
  }


  /**
   * Processing of the configuration file parameters.
   */
  void
  BallDetection::init(Miro::Server& _server, Video::FilterParameters const * _params)
  {
    // Down cast the params pointer to the BallDetection parameters class.
    params_ = dynamic_cast<BallDetectionParameters const *>(_params);
    // Assert that the cast was successfull.
    assert(params_ != NULL);

    // Super class initialization.
    Video::Filter::init(_server, _params);
  }


  /**
   * Ball detection.
   */
  void
  BallDetection::process()
  {
    // Pointer to the input image.
    unsigned char const * src = inputBuffer();
    // Pointer to the output image.
    unsigned char * dest = outputBuffer();
    // Pointer to the output meta data
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
    }
  }
};
