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
#ifndef VideoFilterRGBtoSeg_h
#define VideoFilterRGBtoSeg_h

/*! \file VideoFilterRGBtoSeg.h
 *  \brief Image conversion routines YUV411->RGB (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilter.h"

namespace Video
{

    //! Lookup table driven conversion from YUV411 to RGB
    class FilterRGBtoSeg : public Filter
    {
      typedef Filter Super;



    public:
        FilterRGBtoSeg(Miro::ImageFormatIDL const& _inputFormat);
	virtual ~FilterRGBtoSeg();
	virtual void process();
    protected:

    };
};

#endif // VideoFilterRGBtoSeg_h

