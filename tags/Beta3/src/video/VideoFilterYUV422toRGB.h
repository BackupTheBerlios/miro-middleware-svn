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
#ifndef VideoFilterYUV422toRGB_h
#define VideoFilterYUV422toRGB_h

/*! \file FilterYUV422toRGB.h
 *  \brief Image conversion routines YUV422->RGB (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilterYUVbase.h"

namespace Video
{
  //! Lookup table driven conversion from YUV422 to RGB
  class FilterYUV422toRGB : public FilterYUVbase
  {
    typedef FilterYUVbase Super;
  public:
    FilterYUV422toRGB(Miro::ImageFormatIDL const& _inputFormat);
    virtual ~FilterYUV422toRGB();
    virtual void process();
  };
};

#endif // VideoFilterYUV422toRGB_h

