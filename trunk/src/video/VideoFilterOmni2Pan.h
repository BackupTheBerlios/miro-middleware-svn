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
#ifndef VideoFilterOmni2Pan_h
#define VideoFilterOmni2Pan_h

/*! \file VideoFilterOmni2Pan.h
 *  \brief Image conversion routines YUV411->RGB (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilter.h"

namespace Video
{

    //! Lookup table driven conversion from YUV411 to RGB
    class FilterOmni2Pan : public Filter
    {
      typedef Filter Super;
      static double cos_lookup[360];
      static double sin_lookup[360];
      void buildLookupTables();

    public:
        FilterOmni2Pan(Miro::ImageFormatIDL const& _inputFormat);
	virtual ~FilterOmni2Pan();
	virtual void process();
    protected:
    	int middle_x;
	int middle_y;
    };
};

#endif // VideoFilterOmni2Pan_h

