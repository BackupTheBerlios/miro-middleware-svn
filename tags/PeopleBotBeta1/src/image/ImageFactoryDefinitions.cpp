#include "ImageFactoryDefinitions.h"

namespace MiroImage {

void initializeSharedImageIDL(SharedImageIDL& img)
	{
	img.sharedImageID = -1;
	img.dataValid = false;
	img.sharedMemoryValid = false;
	img.shMemKey = -1;
	img.serverShMemData = 0;
	img.format = 0;
	}

void initializeImageIDL(ImageIDL& img)
	{
	img.width = 0;
	img.height = 0;
	img.imageValid = false;
	img.dataSource = kNoSource;
	img.dataChannel = kNoChannel;
	img.dataSubfields = kAllSubfields;
	img.dataFormats = 0;
	img.sourceFormat = 0;
#ifdef MIRO
	img.timestamp.sec = 0;
	img.timestamp.usec = 0;
#else
	img.timestamp = 0;
#endif
	initializeSharedImageIDL(img.grayImage);
	initializeSharedImageIDL(img.rgbImage);
	initializeSharedImageIDL(img.hsvImage);
	}

void initializeStereoImageIDL(StereoImageIDL& img)
	{
	initializeImageIDL(img.leftImage);
	img.leftImage.dataChannel = kLeftChannel;
	initializeImageIDL(img.rightImage);
	img.rightImage.dataChannel = kRightChannel;
	}

}
