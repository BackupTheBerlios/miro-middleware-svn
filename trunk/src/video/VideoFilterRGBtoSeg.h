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
	FILTER_PARAMETERS_FACTORY(FilterRGBtoSeg);
	
	virtual void process();
    protected:
        void init(Miro::Server& _server, FilterParameters const * _params);
        
        void buildLookupTables();
    
        float r_lookup[256];
	float g_lookup[256];
	float b_lookup[256];
	
	
	unsigned int bitImage_[360*4];
	unsigned int distanceProfile_[360];
	unsigned int distLookup_[120];
        FilterRGBtoSegParameters const  * params_;
    };
};

#endif // VideoFilterRGBtoSeg_h

