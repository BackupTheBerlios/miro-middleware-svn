// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef VideoDevice_h
#define VideoDevice_h

#include "Parameters.h"
#include <ace/Time_Value.h>

#include <string>

namespace Video
{
    class VideoDeviceBase;

//--------------------------------------------------------------------------
//! VideoDevice - Bridge
//--------------------------------------------------------------------------
    class VideoDevice
    {
    public:
	VideoDevice(const std::string&);
	~VideoDevice();

	void	connect();
	void	disconnect();

	int	countBuffers() const;
	int	countFramesCaptured() const;

	int	getFormat() const;
	int	getSource() const;
	int 	getDevicePalette() const;
	int 	getRequestedPalette() const;
	int 	getDeviceSubfield() const;
	int 	getRequestedSubfield() const;

	int	getImageWidth() const;
	int	getImageHeight() const;
	int	getImageSize() const;
	int	getDeviceImageSize() const;

	void *	grabImage(ACE_Time_Value& _timeStamp) const;

    private:
	VideoDeviceBase * workerDevice;

    };
};
#endif
