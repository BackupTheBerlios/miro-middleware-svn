// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "VideoDevice.h"

#include "miro/Exception.h"


namespace 
{
  struct PalettePair 
  {
    std::string first;
    Miro::VideoPaletteIDL second;
  };

  PalettePair palette[Video::Device::NUM_PALETTE_ENTRIES + 2] = {
    {"grey",   Miro::GREY_8},
    {"gray",   Miro::GREY_8},
    {"grey16", Miro::GREY_16},
    {"gray16", Miro::GREY_16},
    {"rgb",    Miro::RGB_24},
    {"bgr",    Miro::BGR_24},
    {"rgba",   Miro::RGB_32},
    {"abgr",   Miro::BGR_32},
    {"yuv",    Miro::YUV_24},
    {"yuv411", Miro::YUV_411},
    {"yuv422", Miro::YUV_422}
  };


};

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(Device);

  //--------------------------------------------------------------------
  Device::Device(const Miro::ImageFormatIDL& _format) :
    Super(_format)
  {
    // clear table of supported palettes
    for (unsigned int i = 0; i < NUM_PALETTE_ENTRIES; ++i)
      paletteLookup[i] = -1;
  }

  //--------------------------------------------------------------------
  Device::~Device()
  {
  }

  //--------------------------------------------------------------------
  void 
  Device::setBuffer(unsigned char *)
  {
    throw Miro::Exception("Device::setBuffer not supported.");
  }
  //--------------------------------------------------------------------
  void
  Device::setInterface(Miro::Server&, VideoInterfaceParameters const &)
  {
    throw Miro::Exception("Device::setInterface not supported.");    
  }

  //--------------------------------------------------------------------
  Miro::VideoPaletteIDL 
  Device::getPalette(const std::string & pal)
  {  
    for (unsigned int i = 0; i < NUM_PALETTE_ENTRIES + 2; ++i) {
      if (pal == palette[i].first) {
	return palette[i].second;
      }
    }

    throw Miro::Exception(std::string("Video::Parameters: unknown palette: ") + pal);
  }
};

