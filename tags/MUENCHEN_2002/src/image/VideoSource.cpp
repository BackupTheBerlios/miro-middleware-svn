#include "Pixels.h"
#include "videoSource.h"

namespace MiroImage {

videoSource::videoSource(const char* l, const char* r, bool force)
	{
	try
		{
		if (l && l[0]>0)
			grabberLeft.open(l);
		if (r && r[0]>0)
			grabberRight.open(r);
		}
	catch (Miro::EDevIO& e)
		{
	#ifdef DEBUG
		std::cerr << "caught exception while opening framegrabber devices" << std::endl;
	#endif	
		if (force)
			{
			//	we couldn't open all requested devices
			grabberLeft.close();
			grabberRight.close();
			throw e;
			}
		else
			{
			if (!(isLeftOpen() || isRightOpen()))
			//	we couldn't open any requested device
				throw e;
			}
		}
	}

videoSource::~videoSource()
	{
	grabberLeft.close();
	grabberRight.close();
	}

void videoSource::requestImage(ImageIDL& img, long tolerance)
	{
#ifdef DEBUG_LOG
	std::cout << "videoSource::requestImage" << std::endl;
#endif	
	parameterCheck(img, tolerance);
	if (img.dataChannel == kRightChannel)
		grabberRight.requestImage(img, tolerance);
	else
		grabberLeft.requestImage(img, tolerance);
#ifdef DEBUG_LOG
	std::cout << "...done" << std::endl;
#endif	
	}

void videoSource::releaseImage(ImageIDL& img)
	{
#ifdef DEBUG_LOG
	std::cout << "videoSource::releaseImage" << std::endl;
#endif	
	if (img.dataChannel == kRightChannel)
		grabberRight.releaseImage(img);
	else
		grabberLeft.releaseImage(img);
#ifdef DEBUG_LOG
	std::cout << "...done" << std::endl;
#endif	
	}
		
booleanType videoSource::isLeftOpen() const
	{
	return grabberLeft.isOpen();
	}

booleanType videoSource::isRightOpen() const
	{
	return grabberRight.isOpen();
	}

/*
	protected:
*/		
void videoSource::getLeftImage(ImageIDL& img)
	{
#ifdef DEBUG_LOG
	std::cout << "videoSource::getLeftImage" << std::endl;
#endif	
	grabberLeft.getImage(img);
	img.dataSource = kVideoSource;
#ifdef DEBUG_LOG
	std::cout << "...done" << std::endl;
#endif	
	}

void videoSource::getRightImage(ImageIDL& img)
	{
#ifdef DEBUG_LOG
	std::cout << "videoSource::getRightImage" << std::endl;
#endif	
	grabberRight.getImage(img);
	img.dataSource = kVideoSource;
#ifdef DEBUG_LOG
	std::cout << "...done" << std::endl;
#endif	
	}

}
