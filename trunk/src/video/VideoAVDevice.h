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
#ifndef VideoAVDevice_h
#define VideoAVDevice_h

#include "VideoDevice.h"

namespace Video
{
  //! Base class for analog video devices.
  /**
   * Analog video devices such as frame grabbers, share some
   * common attributes that are encapsulated within this 
   * base class.
   */
  class AVDevice : public Device
  {
    typedef Device Super;
  public:
    //! The available video formats.
    enum VideoFormat {
      FORMAT_AUTO,
      FORMAT_PAL,
      FORMAT_NTSC,
      FORMAT_SECAM
    };

    //! The available video sources.
    enum VideoSource {
      SOURCE_COMPOSITE1,
      SOURCE_COMPOSITE2,
      SOURCE_COMPOSITE3,
      SOURCE_SVIDEO,
      SOURCE_TUNER
    };

    //! Initializing constructor.
    /** Actually just passes the parameter to the Super class. */
    AVDevice(Miro::ImageFormatIDL const & _inputFormat);

    FILTER_PARAMETERS_FACTORY(AVDevice);

    //! Number of available video formats.
    static const unsigned int NUM_FORMAT_ENTRIES = FORMAT_SECAM + 1;
    //! Number of available video sources.
    static const unsigned int NUM_SOURCE_ENTRIES = SOURCE_TUNER + 1;
    
  protected:
    //! Conversion of string based video format specifier to enumeration type.
    static VideoFormat getFormat(std::string const & _fmt);
    //! Conversion of string based video source specifier to enumeration type.
    static VideoSource getSource(std::string const & _src);

    //! The formats available by the AvDVDevice descendants.
    /** To be initialized on their construction or initialization. */
    int formatLookup[NUM_FORMAT_ENTRIES];
    //! The sources available by the AvDVDevice descendants.
    /** To be initialized on their construction or initialization. */
    int sourceLookup[NUM_SOURCE_ENTRIES];
  };
}

#endif // VideoAVDevice_h
