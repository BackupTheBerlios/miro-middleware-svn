#ifdef macintosh
#include <shm.h>
#else
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#endif

#include "Pixels.h"
#include "imageSource.h"

namespace MiroImage {

imageSource::imageSource()
	{
	supportedSubfields = kEvenSubfield|kOddSubfield;
	supportedChannel = kLeftChannel|kRightChannel;
	}

imageSource::~imageSource()
	{
	}
		
void imageSource::parameterCheck(ImageIDL& img, long tolerance)
	{
	if (!validChannel(img.dataChannel))
		{
		if (tolerance & kChannelTolerance)
			img.dataChannel = img.dataChannel & supportedChannel;
		else
			throw Miro::EOutOfBounds();
		}
	if (!validSubfield(img.dataSubfields))
		{
		if (tolerance & kSubfieldTolerance)
			img.dataSubfields = img.dataSubfields & supportedSubfields;
		else
			throw Miro::EOutOfBounds();
		}
	}
		
void imageSource::parameterCheck(StereoImageIDL& img, long tolerance)
	{
	parameterCheck(img.leftImage, tolerance);
	parameterCheck(img.rightImage, tolerance);
	}

void imageSource::getImage(ImageIDL& img)
	{
	if (img.dataChannel == kRightChannel)
		getRightImage(img);
	else
		getLeftImage(img);
	}

void imageSource::getStereoImage(StereoImageIDL& img)
	{
	getLeftImage(img.leftImage);
	getRightImage(img.rightImage);
	}

booleanType imageSource::validChannel(long c) const
	{
	return (c & supportedChannel) == c;
	}

booleanType imageSource::validSubfield(long c) const
	{
	return (c & supportedSubfields) == c;
	}

}
