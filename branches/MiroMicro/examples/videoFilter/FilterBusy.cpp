// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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

