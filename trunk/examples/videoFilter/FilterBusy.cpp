// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "FilterBusy.h"

#include "miro/VideoHelper.h"

namespace {
  volatile unsigned int v = 1;
}

FILTER_PARAMETERS_FACTORY_IMPL(FilterBusy);

FilterBusy::FilterBusy(const Miro::ImageFormatIDL& _format) :
  Super(_format),
  imageSize_(getImageSize(_format))
{
}

void
FilterBusy::process()
{
  memcpy(outputBuffer(), inputBuffer(), imageSize_);

  FilterBusyParameters const * params =
    dynamic_cast<FilterBusyParameters const *>(params_);
  MIRO_ASSERT(params != NULL);

  for (unsigned int i = 0; i < params->counting; ++i) {
    for (unsigned int j = 0; j < 1000; ++j) {
      v *= j;
    }
  }
}

