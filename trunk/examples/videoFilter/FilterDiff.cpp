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

#include "FilterDiff.h"       // Class declaration.
#include "miro/VideoHelper.h" // Global helper functions.
#include "miro/Exception.h"   // Exception declarations.
#include "miro/Log.h"
#include "video/BufferManager.h" 


// Factory method implementation.
FILTER_PARAMETERS_FACTORY_IMPL(FilterDiff);

/**
 * The filter expects the input format palette to be Miro::RGB_24. Otherwise
 * a Miro::Exception is thrown.
 */
FilterDiff::FilterDiff(Miro::ImageFormatIDL const& _inputFormat) :
  Super(_inputFormat)
{
//   if (inputFormat_.palette != Miro::RGB_24)
//     throw Miro::Exception("FilterDiff: unsupported input format (RGB required).");
  outputFormat_.palette = Miro::GREY_8;

  outputSize_ = Miro::getImageSize(outputFormat_);
}

/**
 * Processing of the configuration file parameters.
 */
void
FilterDiff::init(Miro::Server& _server, Video::FilterParameters const * _params)
{
  // Down cast the params pointer to the FilterGray parameters class.
  params_ = dynamic_cast<FilterDiffParameters const *>(_params);
  // Assert that the cast was successfull.
  assert(params_ != NULL);

  // Super class initialization.
  Super::init(_server, _params);
}

/**
 * Image conversion.
 */
void
FilterDiff::process()
{
  // get additional lock on buffer
  unsigned long index = pre_->bufferManager()->acquireCurrentReadBuffer();
  assert(index == inputBufferIndex());
  // Queue the input image.
  queue_.push_back(FilterLock(inputBufferIndex(), inputBuffer()));

  if (queue_.size() > params_->historySize) {
    // release lock on buffer
    pre_->bufferManager()->releaseReadBuffer(queue_.front().index);
  // Pop the input image.
    queue_.pop_front();
  }

  // Pointer to the first input image.
  unsigned char const * src1 = queue_.front().buffer;
  // Pointer to the second input image.
  unsigned char const * src2 = inputBufferLink(0);
  // Pointer to the output image.
  unsigned char * dest = outputBuffer();
  // Past the end pointer of the output image for termination condition.
  unsigned char * last = dest + outputSize_;

  MIRO_LOG_OSTR(LL_NOTICE, 
		"linked image time: " <<
		preLink_[0].filter()->bufferManager()->bufferTimeStamp(preLink_[0].index()) <<
		std::endl);


  while (dest < last) {
    *(dest) = (*src1 >= *src2)? *src1 - *src2 : *src2 - *src1;
    ++dest;
    ++src1;
    ++src2;
  }
}
