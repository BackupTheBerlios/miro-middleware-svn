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
#ifndef VideoAVDevice_h
#define VideoAVDevice_h

#include "VideoDevice.h"

namespace Video
{
  class AVDevice : public Device
  {
    typedef Device Super;
  public:
    enum VideoFormat {
      FORMAT_AUTO,
      FORMAT_PAL,
      FORMAT_NTSC,
      FORMAT_SECAM
    };

    enum VideoSource {
      SOURCE_COMPOSITE1,
      SOURCE_COMPOSITE2,
      SOURCE_COMPOSITE3,
      SOURCE_SVIDEO,
      SOURCE_TUNER
    };

    AVDevice(Miro::ImageFormatIDL const & _inputFormat);

    FILTER_PARAMETERS_FACTORY(AVDevice);

    static const unsigned int NUM_FORMAT_ENTRIES = FORMAT_SECAM + 1;
    static const unsigned int NUM_SOURCE_ENTRIES = SOURCE_TUNER + 1;
    
  protected:
    static VideoFormat getFormat(std::string const & _fmt);
    static VideoSource getSource(std::string const & _src);

    int formatLookup[NUM_FORMAT_ENTRIES];
    int sourceLookup[NUM_SOURCE_ENTRIES];
  };
};

#endif // VideoAVDevice_h
