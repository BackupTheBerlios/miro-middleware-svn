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

#include "VideoConfig.h"
#include "Parameters.h"

// forward declarations
class ACE_Time_Value;

namespace Video
{
  //--------------------------------------------------------------------------
  //! VideoDevice
  //--------------------------------------------------------------------------
  class VideoDevice
  {
  public:
    VideoDevice(Parameters const * _params = Parameters::instance());
    virtual ~VideoDevice();

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void * grabImage(ACE_Time_Value& _timeStamp) const = 0;

    virtual int	countBuffers() const;
    virtual int	countFramesCaptured() const;

    virtual int	getFormat() const;
    virtual int	getSource() const;
    virtual int getDevicePalette() const;
    virtual int getRequestedPalette() const;
    virtual int getDeviceSubfield() const;
    virtual int getRequestedSubfield() const;

    virtual int	getImageWidth() const;
    virtual int	getImageHeight() const;
    virtual int	getImageSize() const;
    virtual int	getDeviceImageSize() const;

  protected:
    int getPixelSize(const int) const;


    Parameters const * params_;

    int		formatID;
    int		sourceID;
    int		requestedPaletteID;
    int		devicePaletteID;
    int		requestedSubfieldID;
    int		deviceSubfieldID;

    int		imgWidth;
    int		imgHeight;
	
    int		iNBuffers;
    mutable int	iCurrentBuffer;
    mutable int	iNFramesCaptured;

    int		formatLookup[numFormatEntries];
    int		paletteLookup[numPaletteEntries];
    int		sourceLookup[numSourceEntries];
  };
};
#endif
