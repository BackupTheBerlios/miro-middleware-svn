// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoDevice_h
#define VideoDevice_h

#include "VideoFilter.h"

// forward declarations
class ACE_Time_Value;

namespace Video
{
  //--------------------------------------------------------------------------
  //! VideoDevice
  //--------------------------------------------------------------------------
  class Device : public Filter
  {
    typedef Filter Super;

  public:
    Device(Miro::ImageFormatIDL const & _format);
    virtual ~Device();

    FILTER_PARAMETERS_FACTORY(Device);

    virtual void setBuffer(unsigned char *);
    virtual void setInterface(Miro::Server&, VideoInterfaceParameters const &);

    static Miro::VideoPaletteIDL getPalette(std::string const & _pal);

    static const unsigned int NUM_PALETTE_ENTRIES = Miro::YUV_422 + 1;

  protected:
    int getPixelSize(const int) const;

    int paletteLookup[NUM_PALETTE_ENTRIES];
  };
};
#endif
