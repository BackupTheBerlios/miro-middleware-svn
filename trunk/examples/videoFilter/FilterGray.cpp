// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "FilterGray.h"
#include "miro/VideoHelper.h"
#include "miro/Exception.h"

FILTER_PARAMETERS_FACTORY_IMPL(FilterGray);

FilterGray::FilterGray(Miro::ImageFormatIDL const& _inputFormat) :
  Super(_inputFormat)
{
  if (inputFormat_.palette != Miro::RGB_24)
    throw Miro::Exception("FilterGray: unsupported input format (RGB required).");
  outputFormat_.palette = Miro::GREY_8;

  outputSize_ = Miro::getImageSize(outputFormat_);
}

void
FilterGray::init(Video::FilterParameters const * _params)
{
  params_ = dynamic_cast<FilterGrayParameters const *>(_params);
  assert(params_ != NULL);

  sum_ = params_->weightRed + params_->weightGreen + params_->weightBlue;

  Super::init(_params);
}

void
FilterGray::process()
{
  unsigned char const * src = inputBuffer();
  unsigned char * dest = outputBuffer();
  unsigned char * last = dest + outputSize_;

  while (dest < last) {
    *(dest++) = 
      (unsigned char) ( ( *(src++) * params_->weightRed +
			  *(src++) * params_->weightGreen +
			  *(src++) * params_->weightBlue) 
			/ sum_) ;
  }
}
