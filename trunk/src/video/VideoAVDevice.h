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
