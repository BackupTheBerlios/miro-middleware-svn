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
#ifndef VideoFilterYUV411toRGB_h
#define VideoFilterYUV411toRGB_h

/*! \file VideoFilterYUV411toRGB.h
 *  \brief Image conversion routines YUV411->RGB (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilterYUVbase.h"

namespace Video
{
    //! Lookup table driven conversion from YUV411 to RGB
    class FilterYUV411toRGB : public FilterYUVbase
    {
      typedef FilterYUVbase Super;
    public:
        FilterYUV411toRGB(Miro::ImageFormatIDL const& _inputFormat);
	virtual ~FilterYUV411toRGB();
	
	FILTER_PARAMETERS_FACTORY(FilterYUV411toRGB);
	virtual void process();
    };
}

#endif // VideoFilterYUV411toRGB_h

