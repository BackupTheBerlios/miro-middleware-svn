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
#ifndef VideoFilterYUVbase_h
#define VideoFilterYUVbase_h

/*! \file VideoFilterYUVbase.h
 *  \brief Image conversion routines YUV->RGB (interface)
 */

#include "VideoFilter.h"

namespace Video
{
    //! Lookup table driven conversion from YUV422 to RGB
    class FilterYUVbase : public Filter
    {
      typedef Filter Super;
    public:
        FilterYUVbase(Miro::ImageFormatIDL const& _inputFormat);
	virtual ~FilterYUVbase();
	
    protected:
        void initTables();
			
        unsigned char t_r[65536];
	unsigned char t_b[65536];
	unsigned char t_g1[65536];
	unsigned char t_g2[65536];
    };
};

#endif // VideoFilterYUVbase_h

