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
#ifndef VideoFilterYUV411toYUV_h
#define VideoFilterYUV411toYUV_h

/*! \file VideoFilterYUV411toYUV.h
 *  \brief Image conversion routines YUV411->YUV (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilter.h"

namespace Video
{
  //! Lookup table driven conversion from YUV411 to YUV
  class FilterYUV411toYUV : public Filter
  {
    typedef Filter Super;
  public:
    FilterYUV411toYUV(Miro::ImageFormatIDL const& _inputFormat);
    virtual ~FilterYUV411toYUV();
    virtual void process();
  };
};

#endif // VideoFilterYUV411toYUV_h

