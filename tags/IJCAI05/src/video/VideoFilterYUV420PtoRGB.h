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
#ifndef VideoFilterYUV420PtoRGB_h
#define VideoFilterYUV420PtoRGB_h

/*! \file VideoFilterYUV420PtoRGB.h
 *  \brief Image conversion routines YUV411->RGB (interface)
 *  \author Arnd Muehlenfeld
 */

#include "VideoFilter.h"

namespace Video
{

    //! Lookup table driven conversion from YUV411 to RGB
    class FilterYUV420PtoRGB : public Filter
    {
      typedef Filter Super;
      static long int crv_tab[256];
      static long int cbu_tab[256];
      static long int cgu_tab[256];
      static long int cgv_tab[256];
      static long int tab_76309[256];
      static unsigned char clp[1024];
      void InitConvtTbl();

    public:
        FilterYUV420PtoRGB(Miro::ImageFormatIDL const& _inputFormat);
	virtual ~FilterYUV420PtoRGB();

	virtual void process();
    };
};

#endif // VideoFilterYUV420PtoRGB_h

