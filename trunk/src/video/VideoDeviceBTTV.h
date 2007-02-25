// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef VideoDeviceBTTV_h
#define VideoDeviceBTTV_h

extern "C" {
#include <linux/videodev.h>
}

#include "VideoAVDevice.h"

#include <ace/DEV_Connector.h>

namespace Video
{
  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  class DeviceBTTV : public AVDevice
  {
    typedef AVDevice Super;
  public:
    enum VideoSubfield {
        SUBFIELD_ALL,
        SUBFIELD_ODD,
        SUBFIELD_EVEN
    };

    DeviceBTTV(const Miro::ImageFormatIDL& _inputFormatParameters);
    virtual ~DeviceBTTV();

    FILTER_PARAMETERS_FACTORY(DeviceBTTV);

    static const unsigned int NUM_SUBFIELD_ENTRIES = SUBFIELD_EVEN + 1;

  protected:
    virtual BufferManager * bufferManagerInstance() const;
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void fini();

    // methods
    void setFormat();
    void setSource();
    void setPalette();
    void setSize();

    void getCapabilities();
    void getChannels();

    bool probeFormat(int format);
    int probeAllFormats();

    // static methods
    static VideoSubfield getSubfield(std::string const & _sub);

    // data
    DeviceBTTVParameters const * params_;

    ACE_DEV_Addr devName_;
    ACE_DEV_IO ioBuffer_;
    ACE_DEV_Connector connector_;

    struct video_mbuf gb_buffers;
    struct video_capability capability;
    struct video_channel * channels;

    unsigned char * map_;

    int		iNBuffers;
    mutable int	iCurrentBuffer;

    mutable int currentBuffer_;
    mutable int nextBuffer_;

    // static protected data
    //! screen grab
    static int gb_pal[64];
  };
};
#endif
