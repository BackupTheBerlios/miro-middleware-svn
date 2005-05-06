// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoFilterYUV422toYUV_h
#define VideoFilterYUV422toYUV_h

/*! \file VideoFilterYUV422toYUV.h
 *  \brief Image conversion routines YUV422->YUV (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilter.h"

namespace Video
{
  //! Lookup table driven conversion from YUV422 to YUV
  class FilterYUV422toYUV : public Filter
  {
    typedef Filter Super;
  public:
    FilterYUV422toYUV(Miro::ImageFormatIDL const& _inputFormat);
    virtual ~FilterYUV422toYUV();
    virtual void process();
    FILTER_PARAMETERS_FACTORY(FilterYUV422toYUV);
  };
}

#endif // VIDEOCONVERTERYUV422TORGB_H

