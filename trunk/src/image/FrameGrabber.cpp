#ifndef macintosh
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#endif

#include <unistd.h>

#include "Pixels.h"
#include "frameGrabber.h"

namespace MiroImage {

const integerType	maxPALBufferSize = iMaxPALColumns * iMaxPALRows * 4;

frameGrabber::frameGrabber()
	{
	deviceFd = -1;
	iNCaptureRetries = 5;
	pMappedMemory = NULL;
	iMappedMemorySize = 0;
		
	iNRequests = 0;
	iCurrentColumns = 0;
	iCurrentRows = 0;
	iCurrentFormat = 0;
	iCurrentPixelSize = 0;

	iMinColumns = 0;
	iMaxColumns = 0;
		
	iMinRows = 0;
	iMaxRows = 0;
	}

frameGrabber::~frameGrabber()
	{
	}
		
void frameGrabber::getImage(ImageIDL& img)
	{
	if (isOpen())
		{
		img.grayImage.dataValid = false;
		img.rgbImage.dataValid = false;
		img.hsvImage.dataValid = false;
		grabImage(img);
		switch (img.sourceFormat)
			{
			case kRGB:
				copyRGB(img);
				break;
			case kGray:
				copyGray(img);
				break;
			default:
				throw Miro::EOutOfBounds();
				break;
			}
		}
	else
		throw Miro::EDevIO();
	}

bool frameGrabber::isOpen() const
	{
	return deviceFd >= 0;
	}

bool frameGrabber::isMapped() const
	{
	return pMappedMemory != 0;
	}

bool frameGrabber::isRequested() const
	{
	return iNRequests > 0;
	}

void frameGrabber::open(const char* dev)
	{
#ifdef DEBUG
	std::cout << "open device: `" << dev << "'..." << std::endl;
#endif
	//	open video device
	deviceName = dev;
#ifdef macintosh
	deviceFd = -1;
#else
	deviceFd = ::open(deviceName.c_str(), O_RDONLY);
#endif
	if (deviceFd < 0)
		{
#ifdef DEBUG
		std::cout << "...failed" << std::endl;
#endif
		throw Miro::EDevIO();
		}
#ifdef DEBUG
	std::cout << "...done" << std::endl;
#endif
	}

void frameGrabber::close()
	{
	//	close video device
	if (isOpen())
		{
		if (isMapped())
			unmap();
	#ifndef macintosh
		::close(deviceFd);
	#endif
		deviceFd = -1;
		}
	}

void frameGrabber::releaseImage(ImageIDL&)
	{
	if ((iNRequests == 1) && isMapped())
		unmap();
	if (iNRequests > 0)
		iNRequests = iNRequests - 1;
	}
		
void frameGrabber::parameterCheck(ImageIDL& img, long tolerance)
	{
	if (img.width > iMaxColumns)
		{
		if (tolerance & kSizeTolerance)
			img.width = iMaxColumns;
		else
			throw Miro::EOutOfBounds();
		}
	if (img.width < iMinColumns)
		{
		if (tolerance & kSizeTolerance)
			img.width = iMinColumns;
		else
			throw Miro::EOutOfBounds();
		}
	if (img.height > iMaxRows)
		{
		if (tolerance & kSizeTolerance)
			img.height = iMaxRows;
		else
			throw Miro::EOutOfBounds();
		}
	if (img.height < iMinRows)
		{
		if (tolerance & kSizeTolerance)
			img.height = iMinRows;
		else
			throw Miro::EOutOfBounds();
		}
	if (iNRequests > 0)
		modeCheck(img);
	}

/**
	Default behaviour:
		only one size allowed
*/
void frameGrabber::modeCheck(ImageIDL& img)
	{
	if (img.width != iCurrentColumns)
		throw Miro::EOutOfBounds();
	if (img.height != iCurrentRows)
		throw Miro::EOutOfBounds();
	}

void frameGrabber::map(integerType s, booleanType rw)
	{
	if (iMappedMemorySize != s)
		unmap();

	if (!isMapped())
		{
		#ifdef macintosh
		pMappedMemory = (void*)-1;
		#else
		if (rw)
			pMappedMemory = ::mmap((caddr_t)0, s, PROT_READ | PROT_WRITE, MAP_FILE|MAP_PRIVATE, deviceFd, (off_t)0);
		else
			pMappedMemory = ::mmap((caddr_t)0, s, PROT_READ, MAP_FILE|MAP_PRIVATE, deviceFd, (off_t)0);
		#endif
		if (pMappedMemory == (void*)-1)
			{
			pMappedMemory = NULL;
			throw Miro::EDevIO();
			}
		iMappedMemorySize = s;
		}
	}

void frameGrabber::unmap()
	{
	if (isMapped())
		{
		#ifndef macintosh
		::munmap(pMappedMemory, iMappedMemorySize);
		pMappedMemory = NULL;
		iMappedMemorySize = 0;
		#endif
		}
	}

}
