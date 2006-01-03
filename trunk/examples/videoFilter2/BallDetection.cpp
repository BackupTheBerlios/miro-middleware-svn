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
    Super(_inputFormat)
  {
    if (inputFormat_.palette != Miro::RGB_24)
      throw Miro::Exception("BallDetection: unsupported input format (RGB required).");
    outputFormat_.palette = Miro::GREY_8;
  }

  /**
   * Ball detection.
   */
  void
  BallDetection::process()
  {
    // Pointer to the filter parameters.
    Parameters const * params = dynamic_cast<Parameters const *>(params_);
    // Pointer to the input image.
    unsigned char const * src = inputBuffer();
    // Pointer to the output image.
    unsigned char * dest = outputBuffer();
    // Pointer to the output meta data
    Video::FilterImageParameters  * imageParams = outputBufferParameters();
    ImageParameters * output = dynamic_cast<ImageParameters *>(imageParams);

    int sum = 0, sumX = 0, sumY = 0;
    for (unsigned int x=0; x < inputFormat_.width; ++x) {
      for (unsigned int y=0; y < inputFormat_.height; ++y) {
	if ((src[y*inputFormat_.width*3+x*3] <= params->redHigh) && 
	    (src[y*inputFormat_.width*3+x*3] >= params->redLow) &&
	    (src[y*inputFormat_.width*3+x*3+1] <= params->greenHigh) && 
	    (src[y*inputFormat_.width*3+x*3+1] >= params->greenLow) &&
	    (src[y*inputFormat_.width*3+x*3+2] <= params->blueHigh) && 
	    (src[y*inputFormat_.width*3+x*3+2] >= params->blueLow)) {
	  sum++;
	  sumX += x;
	  sumY += y;
	  dest[y*inputFormat_.width+x] = 255;
	} else {
	  dest[y*inputFormat_.width+x] = 0;
	}
      }
    }
    if (sum > params->redRatio * inputFormat_.width * inputFormat_.height) {
      output->x = sumX / sum;
      output->y = sumY / sum;
    }
  }
}
