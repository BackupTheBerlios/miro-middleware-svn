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

#include <string>

namespace Video {

enum
	{
	formatAuto = 0,
	formatPAL,
	formatNTSC,
	formatSECAM
	};

#define numFormatEntries  (formatSECAM + 1)

enum
	{
	paletteGrey = 0,
	paletteRGB,
	paletteRGBA,
	paletteBGR,
	paletteABGR
	};

#define numPaletteEntries  (paletteABGR + 1)

enum
	{
	subfieldAll = 0,
	subfieldOdd,
	subfieldEven
	};

#define numSubfieldEntries  (subfieldEven + 1)

enum
	{
	sourceComposite1 = 0,
	sourceComposite2,
	sourceComposite3,
 	sourceSVideo,
 	sourceTuner,
	sourceUSB,
	source1394
	};

#define numSourceEntries  (source1394 + 1)
//--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
class VideoDevice
	{
	public:
		VideoDevice(const std::string&);
		virtual	~VideoDevice();

		virtual	void	connect(int fd, int buffers, const std::string& fmt, const std::string& src,
					const std::string& pal, const std::string& sub, int w, int h);
		virtual	void	disconnect();

		virtual	int		countBuffers() const;
		virtual	int		countFramesCaptured() const;

		virtual	int		getFormat() const;
		virtual	int		getSource() const;
		virtual	int 	getDevicePalette() const;
		virtual	int 	getRequestedPalette() const;
		virtual	int 	getDeviceSubfield() const;
		virtual	int 	getRequestedSubfield() const;

		virtual	int		getImageWidth() const;
		virtual	int		getImageHeight() const;
		virtual	int		getImageSize() const;
		virtual	int		getDeviceImageSize() const;

		virtual	void*	grabImage() const;

	protected:
		virtual	int	getPixelSize(const int) const;
		virtual	void	handleConnect(const int fd, const int buffers, const int fmt, const int src, const int pal, const int sub, const int w, const int h);
		virtual	void	handleDisconnect();

		VideoDevice();

		VideoDevice*	workerDevice;

		int				formatID;
		int				sourceID;
		int				requestedPaletteID;
		int				devicePaletteID;
		int				requestedSubfieldID;
		int				deviceSubfieldID;

		int				imgWidth;
		int				imgHeight;

		int				iNBuffers;
		mutable int		iCurrentBuffer;
		mutable int		iNFramesCaptured;

		int				formatLookup[numFormatEntries];
		int				paletteLookup[numPaletteEntries];
		int				sourceLookup[numSourceEntries];
  };
};
#endif
