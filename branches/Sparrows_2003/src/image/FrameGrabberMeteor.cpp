#ifndef macintosh
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#endif

#include <cstring>

#include "frameGrabberMeteor.h"
#include "Pixels.h"

namespace MiroImage {

frameGrabberMeteor::frameGrabberMeteor(integerType buffers)
	{
	if (staticMeteorGrabberCount >= MAX_CONCURRENT_METEOR_GRABBERS)
		throw Miro::EOutOfBounds();

	iGrabberIndex = staticMeteorGrabberCount++;
	frameGrabberMeteorSelf[iGrabberIndex] = this;
	
	iFrameSize = 0;
	iNextFrame = 0;
	iCurrentFrame = 0;
	iNFrames = buffers;
	iCurrentFrame = 0;
	bFrameIsAvailable = false;
	iCurrentFrameTotal = 0;
	bMeteorContinous = false;
	//	bMeteorContinous = (iNFrames > 1);
	
#ifdef MIRO
	timestamps = new Miro::TimeIDL[iNFrames];
#else
	timestamps = new uint32[iNFrames];
#endif
	if (!timestamps)
		throw Miro::EOutOfBounds();
	
	iMinColumns = 32;
	iMaxColumns = iMaxPALColumns/2;
		
	iMinRows = 32;
	iMaxRows = iMaxPALRows/2;
	
	meteorMemory = NULL;
	meteorSource = METEOR_INPUT_DEV0;
	meteorInputFormat = METEOR_FMT_PAL;
	}

frameGrabberMeteor::~frameGrabberMeteor()
	{
	close();
#ifdef DEBUG
	if (iNRequests != 0)
		std::cerr << "~patternSource(): iNRequests != 0, iNRequests == " << iNRequests << std::endl;
#endif
	}

void frameGrabberMeteor::requestImage(ImageIDL& img, long tolerance)
	{
	parameterCheck(img, tolerance);

	meteorGeometry.rows =  img.height;
	if ((img.dataSubfields == kEvenSubfield) || (img.dataSubfields == kOddSubfield))
		meteorGeometry.rows = meteorGeometry.rows * 2;
	meteorGeometry.columns = img.width;

	if (bMeteorContinous)
		meteorGeometry.frames = iNFrames;
	else
		meteorGeometry.frames = 1;

	if (iNRequests == 0)
		{
		if (((img.dataFormats & kRGB) == kRGB) || ((img.dataFormats & kHSV) == kHSV))
			{
			meteorGeometry.oformat = METEOR_GEO_RGB24;
			iCurrentPixelSize = 4;
			img.sourceFormat = kRGB;
			}
		else if ((img.dataFormats & kGray) == kGray)
			{
			meteorGeometry.oformat = METEOR_GEO_YUV_PLANAR;
			iCurrentPixelSize = 2;
			img.sourceFormat = kGray;
			}
		else
			throw Miro::EOutOfBounds();

		#ifdef DEBUG_LOG
		std::cout << "set parameter geo" << std::endl;
		#endif
		if (::ioctl(deviceFd, METEORSETGEO, &meteorGeometry) < 0)
			throw Miro::EDevIO();
		#ifdef DEBUG_LOG
		std::cout << "...done" << std::endl;
		std::cout << "set parameter meteorSource" << std::endl;		
		#endif
		if (::ioctl(deviceFd, METEORSINPUT, &meteorSource) < 0)
			throw Miro::EDevIO();
		#ifdef DEBUG_LOG
		std::cout << "...done" << std::endl;
		std::cout << "set parameter meteorInputFormat" << std::endl;		
		#endif
		if (::ioctl(deviceFd, METEORSFMT, &meteorInputFormat) < 0)
			throw Miro::EDevIO();
		#ifdef DEBUG_LOG
		std::cout << "...done" << std::endl;
		#endif

		iFrameSize = meteorGeometry.columns * meteorGeometry.rows * iCurrentPixelSize;
			
		if (bMeteorContinous)
			{
			struct meteor_capframe	capframe;
			integerType		mapSize;
			
			mapSize = (((iFrameSize * iNFrames) + 4095)/4096) * 4096;
			map(mapSize + 4096, true);

			meteorMemory = (struct meteor_mem *) ((byte*)pMappedMemory + mapSize);

		#ifdef DEBUG_LOG
			std::cout << "mmap: " << meteorGeometry.columns << " x " << meteorGeometry.rows << " ( " << iFrameSize
					<< " [ " << (integerType)meteorMemory->frame_size << " ], total == " << mapSize
					<< " byte ), grabber index == " << iGrabberIndex << std::endl;
		#endif
			
			switch (iGrabberIndex)
				{
				case 0:
					signal(SIGUSR1, usr1SignalHandler);
					break;
				case 1:
					signal(SIGUSR2, usr2SignalHandler);
					break;
				default:
					unmap();
					throw Miro::EOutOfBounds();
					break;
				};

			switch (iGrabberIndex)
				{
				unsigned int	sig;

				case 0:
					sig = SIGUSR1;
					if (::ioctl(deviceFd, METEORSSIGNAL, &sig) < 0)
						{
					#ifdef DEBUG
						std::cerr << "unable to set signal" << std::endl;
					#endif
						throw Miro::EDevIO();
						}
					break;
				case 1:
					sig = SIGUSR2;
					if (::ioctl(deviceFd, METEORSSIGNAL, &sig) < 0)
						{
					#ifdef DEBUG
						std::cerr << "unable to set signal" << std::endl;
					#endif
						throw Miro::EDevIO();
						}
					break;
				};
			capframe.command = METEOR_CAP_N_FRAMES;
			capframe.lowat = iNFrames - 4;	//	lo water mark
			capframe.hiwat = iNFrames - 2;	//	hi water mark

			if (::ioctl(deviceFd, METEORCAPFRM, &capframe) < 0)
				throw Miro::EDevIO();
			
			std::cout << "frame_size == " << meteorMemory->frame_size << std::endl	
			 << "num_bufs == " << meteorMemory->num_bufs << std::endl	
			 << "frames_captured == " << meteorMemory->frames_captured << std::endl	
			 << "cur_frame == " << meteorMemory->cur_frame << std::endl	
			 << "cur_field == " << meteorMemory->cur_field << std::endl	
			 << "lowat == " << meteorMemory->lowat << std::endl	
			 << "hiwat == " << meteorMemory->hiwat << std::endl	
			 << "num_active_bufs == " << meteorMemory->num_active_bufs << std::endl;	
				
			}
		else
			map(iFrameSize, false);

		iCurrentColumns = img.width;
		iCurrentRows = img.height;
		}
	iNRequests = iNRequests + 1;
	}


void frameGrabberMeteor::releaseImage(ImageIDL& img)
	{
	if (iNRequests >= 0)
		{
		iNRequests--;
		if (iNRequests == 0)
			{
			if (bMeteorContinous)
				{
				struct meteor_capframe	capframe;

				capframe.command = METEOR_CAP_STOP_FRAMES;
				if  (::ioctl(deviceFd, METEORCAPFRM, &capframe) < 0)
					{
				#ifdef DEBUG
					std::cerr << "frameGrabberMeteor::releaseImage(() : METEOR_CAP_STOP_FRAMES failed" << std::endl;
				#endif
					throw Miro::EDevIO();
					}
			#ifdef DEBUG_LOG
				std::cout << "frameGrabberMeteor::releaseImage((): METEOR_CAP_STOP_FRAMES suceeded" << std::endl;
			#endif
				}
			frameGrabber::releaseImage(img);
			}
		}
	}

/*	
protected:
*/
booleanType	frameGrabberMeteor::imageAvailable()
	{
	return bFrameIsAvailable;
	}

void frameGrabberMeteor::grabImage(ImageIDL& img)
	{
	if (!isOpen() || !isRequested())
		{
	#ifdef DEBUG
		std::cerr << "frameGrabberMeteor::grabImage(): device not open or no image requested" << std::endl;
	#endif
		throw Miro::EDevIO();
		}

	if (bMeteorContinous)
		{
		//	if (!bFrameIsAvailable)
		//		::sleep(1);
		while (!imageAvailable())
			;
		iCurrentFrameSave = iCurrentFrame;
		img.timestamp = timestamps[iCurrentFrameSave];
	#ifdef DEBUG_LOG
		std::cout << "continuous capture: one frame available" << std::endl;
	#endif
		}
	else
		{
		integerType	iNTries = 0;
		booleanType	done = false;
		//	capture one frame
	#ifdef DEBUG_LOG
		std::cout << "capture one frame" << std::endl;
	#endif
		
		int	command = METEOR_CAP_SINGLE;
		integerType	errorCount = getCurrentErrorCount();

		if (::ioctl(deviceFd, METEORCAPTUR, &command) < 0)
			throw Miro::EDevIO();
		
		while (!done && iNCaptureRetries && (iNTries < iNCaptureRetries))
			{
			integerType	currentErrorCount = getCurrentErrorCount();
			if (currentErrorCount == errorCount)
				done = true;
			else
				{
				iNTries++;
				errorCount = currentErrorCount;
				command = METEOR_CAP_SINGLE;
				if (::ioctl(deviceFd, METEORCAPTUR, &command) < 0)
					throw Miro::EDevIO();
				}
			}
		struct	timeval	time;
		::gettimeofday( &time, 0 );
		#ifdef MIRO
		img.timestamp.sec = time.tv_sec;
		img.timestamp.usec = time.tv_usec;
		#else
		img.timestamp = time.tv_sec*1000000 + time.tv_usec;
		#endif
	#ifdef DEBUG
		if (done)
			{
			std::cout << "...done after " << (integerType)iNTries << " retries" << std::endl;
			}
		else
			{
			std::cout << "...failed" << std::endl;
			throw Miro::EDevIO();
			}
	#else
		if (!done)
			throw Miro::EDevIO();
	#endif
		}
	}
	
/*	
protected:
*/
void frameGrabberMeteor::copyRGB(ImageIDL& img)
	{
	integerType	iCurrentFrameTotalSave = iCurrentFrameTotal;
	pixelRGB32behind24LE*	pDataSrc;
	integerType				ix, iy;
	integerType				lineOffset;
	
	#ifdef DEBUG_LOG
		std::cout << "copy rgb..." << std::endl;
	#endif
	if (bMeteorContinous)
		pDataSrc = (pixelRGB32behind24LE*)((byte*)pMappedMemory + (iCurrentFrameSave * iFrameSize));
	else
		pDataSrc = pDataSrc = (pixelRGB32behind24LE*)pMappedMemory;
	
	if (img.dataSubfields == kEvenSubfield)
		{
		lineOffset = img.width;
		}
	else if (img.dataSubfields == kOddSubfield)
		{
		pDataSrc += img.width;
		lineOffset = img.width;
		}
	else
		lineOffset = 0;
		
	if (((img.dataFormats & kRGB) == kRGB) && img.rgbImage.sharedMemoryValid)
		{
		rgb24*	pRGB = (rgb24*)img.rgbImage.serverShMemData;
		
		for (iy = 0; iy < img.height; iy++)
			{
			for (ix = 0; ix < img.width; ix++)
				*pRGB++ = *pDataSrc++;
			pDataSrc += lineOffset;
			}

		img.rgbImage.dataValid = true;
		img.sourceFormat = kRGB;
		}
	else if (((img.dataFormats & kHSV) == kHSV) && img.hsvImage.sharedMemoryValid)
		{
		hsv32*	pHSV = (hsv32*)img.hsvImage.serverShMemData;
		
		for (iy = 0; iy < img.height; iy++)
			{
			for (ix = 0; ix < img.width; ix++)
				{
				pixelRGB32behind24LE	p(*pDataSrc++);
				hsv32::rgb2hsv(*pHSV++, p.r, p.g, p.b);
				}
			pDataSrc += lineOffset;
			}

		img.hsvImage.dataValid = true;
		img.sourceFormat = kHSV;
		}
	else if (((img.dataFormats & kGray) == kGray) && img.grayImage.sharedMemoryValid)
		{
		gray*	pGray = (gray*)img.grayImage.serverShMemData;
		
		for (iy = 0; iy < img.height; iy++)
			{
			for (ix = 0; ix < img.width; ix++)
				*pGray++ = *pDataSrc++;
			pDataSrc += lineOffset;
			}

		img.grayImage.dataValid = true;
		img.sourceFormat = kGray;
		}
	
	//	is there a new grabbed image available ?
	bFrameIsAvailable = (iCurrentFrameTotal > iCurrentFrameTotalSave);
#ifdef DEBUG_LOG
	std::cout << "...done" << std::endl;
#endif
	}

void frameGrabberMeteor::copyGray(ImageIDL& img)
	{
	integerType		iCurrentFrameTotalSave = iCurrentFrameTotal;
	integerType		ix, iy;
	gray*			pDataSrc;
	integerType		lineOffset;
	
#ifdef DEBUG_LOG
	std::cout << "copy gray..." << std::endl;
#endif
	if (bMeteorContinous)
		pDataSrc = (gray*)((byte*)pMappedMemory + (iCurrentFrameSave * iFrameSize));
	else
		pDataSrc = (gray*)pMappedMemory;

	if (img.dataSubfields == kEvenSubfield)
		{
		lineOffset = img.width;
		}
	else if (img.dataSubfields == kOddSubfield)
		{
		pDataSrc += img.width;
		lineOffset = img.width;
		}
	else
		lineOffset = 0;
		
#ifdef DEBUG_LOG
	std::cout << "   w = " << img.width << ", h = " << img.height << ", pDataSrc == " << pDataSrc << ", lineOffset == " << lineOffset
			<< ", iCurrentFrameSave == " << iCurrentFrameSave << ", iFrameSize = " << iFrameSize << std::endl;
			//	<< " [ " << (integerType)meteorMemory->frame_size << " ]"  << std::endl;
#endif
	if (((img.dataFormats & kGray) == kGray) && img.grayImage.sharedMemoryValid)
		{
		gray*	pGray = (gray*)img.grayImage.serverShMemData;
		
		if (lineOffset > 0)
			{
			for (iy = 0; iy < img.height; iy++)
				{
				std::memcpy(pGray, pDataSrc, img.width);
				pGray += img.width;
				pDataSrc += img.width;
				pDataSrc += lineOffset;
				}
			}
		else
			std::memcpy(pGray, pDataSrc, img.height * img.width);


		img.grayImage.dataValid = true;
		img.sourceFormat = kGray;
		}
	else if (((img.dataFormats & kRGB) == kRGB) && img.rgbImage.sharedMemoryValid)
		{
		rgb24*	pRGB = (rgb24*)img.rgbImage.serverShMemData;
		
		for (iy = 0; iy < img.height; iy++)
			{
			for (ix = 0; ix < img.width; ix++)
				*pRGB++ = *pDataSrc++;
			pDataSrc += lineOffset;
			}

		img.rgbImage.dataValid = true;
		img.sourceFormat = kRGB;
		}
	else if (((img.dataFormats & kHSV) == kHSV) && img.hsvImage.sharedMemoryValid)
		{
		hsv32*	pHSV = (hsv32*)img.hsvImage.serverShMemData;
		
		for (iy = 0; iy < img.height; iy++)
			{
			for (ix = 0; ix < img.width; ix++)
				*pHSV++ = *pDataSrc++;
			pDataSrc += lineOffset;
			}

		img.hsvImage.dataValid = true;
		img.sourceFormat = kHSV;
		}
	
	//	is there a new grabbed image available ?
	bFrameIsAvailable = (iCurrentFrameTotal > iCurrentFrameTotalSave);
#ifdef DEBUG_LOG
	std::cout << "...done" << std::endl;
#endif
	}

integerType	frameGrabberMeteor::staticMeteorGrabberCount = 0;

frameGrabberMeteor*	frameGrabberMeteor::frameGrabberMeteorSelf[MAX_CONCURRENT_METEOR_GRABBERS] = {NULL, NULL};

void usr1SignalHandler(int);
void usr2SignalHandler(int);

void usr1SignalHandler(int)
	{
	frameGrabberMeteor*	grabber = frameGrabberMeteor::frameGrabberMeteorSelf[0];
	
	//	Check for valid pointers
	if (grabber && grabber->iNRequests > 0)
		{
		signal(SIGUSR1, usr1SignalHandler);
		grabber->handleSignal();
		}
	#ifdef DEBUG
	if (!grabber)
		std::cerr << "frameGrabberMeteor::usr1SignalHandler(): frameGrabberMeteorSelf[0] == NULL" << std::endl;
	#endif
	}

void usr2SignalHandler(int)
	{
	frameGrabberMeteor*	grabber = frameGrabberMeteor::frameGrabberMeteorSelf[1];
	
	signal(SIGUSR2, usr2SignalHandler);
	//	Check for valid pointers
	if (grabber)
		grabber->handleSignal();
	}

void frameGrabberMeteor::handleSignal()
	{
	#ifdef DEBUG
	if (!meteorMemory)
		std::cerr << "frameGrabberMeteor::handleSignal(): meteorMemory* == NULL" << std::endl;
	#endif
	if (meteorMemory)
		{
		iCurrentFrameTotal++;
		iCurrentFrame = iNextFrame;

	#ifndef macintosh
		struct	timeval	time;
		::gettimeofday( &time, 0 );
		#ifdef MIRO
		timestamps[iCurrentFrame].sec = time.tv_sec;
		timestamps[iCurrentFrame].usec = time.tv_usec;
		#else
		timestamps[iCurrentFrame] = time.tv_sec*1000000 + time.tv_usec;
		#endif
	#else
		uint32	usecs = 42;
		timestamps[iCurrentFrame] = usecs;
	#endif
		iNextFrame = (iNextFrame + 1) % iNFrames;
		bFrameIsAvailable = true;
	#ifdef DEBUG_LOG
		std::cout << "frameGrabberMeteor::handleSignal(): iCurrentFrameTotal = " << iCurrentFrameTotal
				<< ", iCurrentFrame = " << iCurrentFrame << std::endl;
			
			std::cout << "frame_size == " << meteorMemory->frame_size << std::endl	
			 << "num_bufs == " << meteorMemory->num_bufs << std::endl	
			 << "frames_captured == " << meteorMemory->frames_captured << std::endl	
			 << "cur_frame == " << meteorMemory->cur_frame << std::endl	
			 << "cur_field == " << meteorMemory->cur_field << std::endl	
			 << "lowat == " << meteorMemory->lowat << std::endl	
			 << "hiwat == " << meteorMemory->hiwat << std::endl	
			 << "num_active_bufs == " << meteorMemory->num_active_bufs << std::endl;	
	#endif
		meteorMemory->active = (1 << iCurrentFrame);
		meteorMemory->num_active_bufs = 1;
		}
	}

integerType frameGrabberMeteor::getCurrentErrorCount()
	{
#ifdef macintosh
	return 0;
#else
	struct meteor_counts	cnts;
	if (::ioctl(deviceFd, METEORGCOUNT, &cnts) < 0)
		{
	#ifdef DEBUG
		std::cerr << "getCurrentErrorCount() failed" << std::endl;
	#endif
		throw Miro::EDevIO();
		}
	return cnts.fifo_errors + cnts.dma_errors;
#endif
	}

}
