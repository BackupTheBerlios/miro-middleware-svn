/*! \file VideoDeviceBase.h
 *  \brief Base class for a video device (interface)
 *
 * -----------------------------------------------------
 *
 * RoboCup Middle Size Team, Technische Universitaet Graz
 * "Mostly Harmless" 
 * http://www.robocup.tugraz.at
 *
 * Project: Miro::Video
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 * $Log$
 * Revision 1.1  2003/05/12 11:48:05  hutz
 * added the fire wire dc support from graz
 * propagated changes and cleanups introduced by fire wire dc
 * cleaned up the parameter handling of fire wire dc
 *
 * Revision 1.2  2003/03/08 09:24:36  muehlenf
 * Now Video::Parameters are passed through to VideoDevice.
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#ifndef VIDEODEVICEBASE_H
#define VIDEODEVICEBASE_H

#include "VideoDevice.h"
#include "VideoConfig.h"
#include "Parameters.h"

namespace Video
{
    //--------------------------------------------------------------------------
    //! Abstract base class for all VideoDevices.
    //--------------------------------------------------------------------------
    class VideoDeviceBase
    {
    public:
	VideoDeviceBase();
	virtual	~VideoDeviceBase();

	virtual	int	countBuffers() const;
	virtual	int	countFramesCaptured() const;

	virtual	int	getFormat() const;
	virtual	int	getSource() const;
	virtual	int 	getDevicePalette() const;
	virtual	int 	getRequestedPalette() const;
	virtual	int 	getDeviceSubfield() const;
	virtual	int 	getRequestedSubfield() const;
	virtual	int	getImageWidth() const;
	virtual	int	getImageHeight() const;
	virtual	int	getImageSize() const;
	virtual	int	getDeviceImageSize() const;

	virtual	int	getPixelSize(const int) const;

	virtual	void*	grabImage(ACE_Time_Value & _timeStamp) const = 0;
	virtual	void	handleConnect(const int fd, const Parameters& params) = 0;
	virtual	void	handleDisconnect() = 0;

    protected:
	const Parameters * params_;

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

#endif // VIDEODEVICEBASE_H

