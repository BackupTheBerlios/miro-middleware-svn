#ifdef macintosh
#include <shm.h>
#else
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#endif

#include "ImageFactory.h"

#include "imageSource.h"
#include "videoSource.h"
#include "patternSource.h"
#include "Pixels.h"

namespace MiroImage 
{

ImageFactory::ImageFactory()
	{
	monoChannel = kLeftChannel;
	video = NULL;
	pattern = new patternSource;
	render = NULL;

	try
		{
	#ifdef VIDEO_V4L
		video = new videoSource("/dev/video0", "");
	#endif
	#ifdef VIDEO_METEOR
		video = new videoSource("/dev/mmetfgrab0", "/dev/mmetfgrab1");
	#endif
		}
	catch (Miro::EDevIO& e)
		{
		if (true)
			video = pattern;
		else
			throw e;
		}
	
	if (video)
		video->attach();
	if (pattern)
		pattern->attach();
	if (render)
		render->attach();
	}

ImageFactory::~ImageFactory()
	{
	if (video)
		video->detach();
	if (pattern)
		pattern->detach();
	if (render)
		render->detach();
	}

ImageIDL ImageFactory::getImage(long w, long h, long formats,
											long tolerance, long source, long subfields)
	{
	if (monoChannel == kRightChannel)
		return getRightImage(w, h, formats, tolerance, source, subfields);
	else
		return getLeftImage(w, h, formats, tolerance, source, subfields);
	}

ImageIDL ImageFactory::getLeftImage(long w, long h, long formats,
											long tolerance, long source, long subfields)
	{
	ImageIDL	img;
	
	img = requestLeftImage(w, h, formats, tolerance, source, subfields);
	updateImage(img);
	return img;
	}

ImageIDL ImageFactory::getRightImage(long w, long h, long formats,
											long tolerance, long source, long subfields)
	{
	ImageIDL	img;
	
	img = requestRightImage(w, h, formats, tolerance, source, subfields);
	updateImage(img);
	return img;
	}

StereoImageIDL ImageFactory::getStereoImage(long w, long h, long formats,
											long tolerance, long source, long subfields)
	{
	StereoImageIDL	img;
	
	img = requestStereoImage(w, h, formats, tolerance, source, subfields);
	updateStereoImage(img);
	return img;
	}

ImageIDL ImageFactory::requestImage(long w, long h, long formats,
											long tolerance, long source, long subfields)
	{
	if (monoChannel == kRightChannel)
		return requestRightImage(w, h, formats, tolerance, source, subfields);
	else
		return requestLeftImage(w, h, formats, tolerance, source, subfields);
	}

ImageIDL ImageFactory::requestLeftImage(long w, long h, long formats,
											long tolerance, long source, long subfields)
	{
	ImageIDL	img;
#ifdef DEBUG
	std::cout << "requestLeftImage" << std::endl;
#endif
	initializeImageIDL(img);

	img.width = w;
	img.height = h;
	
	img.dataSource = source;
	img.dataChannel = kLeftChannel;
	img.dataSubfields = subfields;
	img.dataFormats = formats;
	
	handleRequestImage(img, tolerance);
	
#ifdef DEBUG
	std::cout << "...done" << std::endl;
#endif
	return img;
	}

ImageIDL ImageFactory::requestRightImage(long w, long h, long formats,
											long tolerance, long source, long subfields)
	{
	ImageIDL	img;

	initializeImageIDL(img);

	img.width = w;
	img.height = h;
	
	img.dataSource = source;
	img.dataChannel = kRightChannel;
	img.dataSubfields = subfields;
	img.dataFormats = formats;
	
	handleRequestImage(img, tolerance);

	return img;
	}

StereoImageIDL ImageFactory::requestStereoImage(long w, long h, long formats,
											long tolerance, long source, long subfields)
	{
	StereoImageIDL	img;

	initializeStereoImageIDL(img);

	img.leftImage.width = w;
	img.leftImage.height = h;
	
	img.leftImage.dataSource = source;
	img.leftImage.dataChannel = kLeftChannel;
	img.leftImage.dataSubfields = subfields;
	img.leftImage.dataFormats = formats;

	img.rightImage.width = w;
	img.rightImage.height = h;
	
	img.rightImage.dataSource = source;
	img.rightImage.dataChannel = kRightChannel;
	img.rightImage.dataSubfields = subfields;
	img.rightImage.dataFormats = formats;
	
	handleRequestImage(img.leftImage, tolerance);
	handleRequestImage(img.rightImage, tolerance);

	return img;
	}

void ImageFactory::updateImage(ImageIDL& img)
	{
#ifdef DEBUG
	std::cout << "updateImage" << std::endl;
#endif
	switch (img.dataSource)
		{
		case kVideoSource:
			videoUpdateImage(img);
			break;
		case kTestPatternSource:
			patternUpdateImage(img);
			break;
		case kRenderSource:
			renderUpdateImage(img);
			break;
		default:
			throw Miro::EOutOfBounds();
		}
	updateImageIDL(img);
	img.imageValid = true;
#ifdef DEBUG
	std::cout << "...done" << std::endl;
#endif
	}

void ImageFactory::updateStereoImage(StereoImageIDL& img)
	{
	updateImage(img.leftImage);
	updateImage(img.rightImage);
	}

void ImageFactory::releaseImage(ImageIDL& img)
	{
	handleReleaseImage(img);
	initializeImageIDL(img);
	}

void ImageFactory::releaseStereoImage(StereoImageIDL& img)
	{
	handleReleaseImage(img.leftImage);
	handleReleaseImage(img.rightImage);
	initializeStereoImageIDL(img);
	}

/*
	private:
*/

void ImageFactory::handleRequestImage(ImageIDL& img, long tolerance)
	{
	switch (img.dataSource)
		{
		case kVideoSource:
			if (video)
				video->requestImage(img, tolerance);
			break;
		case kTestPatternSource:
			if (pattern)
				pattern->requestImage(img, tolerance);
			break;
		case kRenderSource:
			if (render)
				render->requestImage(img, tolerance);
			break;
		default:
			throw Miro::EOutOfBounds();
		}

	allocateImage(img);
	}

void ImageFactory::handleReleaseImage(ImageIDL& img)
	{
	deallocateImage(img);

	switch (img.dataSource)
		{
		case kVideoSource:
			if (video)
				video->releaseImage(img);
			break;
		case kTestPatternSource:
			if (pattern)
				pattern->releaseImage(img);
			break;
		case kRenderSource:
			if (render)
				render->releaseImage(img);
			break;
		default:
			throw Miro::EOutOfBounds();
		}
	}

void ImageFactory::videoUpdateImage(ImageIDL& img)
	{
	if (video)
		video->getImage(img);
	}

void ImageFactory::patternUpdateImage(ImageIDL& img)
	{
	if (pattern)
		pattern->getImage(img);
	}

void ImageFactory::renderUpdateImage(ImageIDL& img)
	{
	if (render)
		render->getImage(img);
	}

void ImageFactory::updateImageIDL(ImageIDL& img)
	{
	switch (img.sourceFormat)
		{
		case kGray:
			updateImageIDLFromGray(img);
			break;
		case kRGB:
			updateImageIDLFromRGB(img);
			break;
		case kHSV:
			updateImageIDLFromHSV(img);
			break;
		default:
			throw Miro::EOutOfBounds();
			break;
		}
	}

void ImageFactory::updateImageIDLFromGray(ImageIDL& img)
	{
	long	nPixel;
	gray*	pGray;
	rgb24*	pRGB;
	hsv32*	pHSV;

	if (img.grayImage.sharedMemoryValid && img.grayImage.dataValid)
		{
		pGray = (gray*)::shmat(img.grayImage.shMemKey, NULL, 0);
		if ((img.dataFormats & kRGB) == kRGB)
			{
			pRGB = (rgb24*)::shmat(img.rgbImage.shMemKey, NULL, 0);
			nPixel = img.width * img.height;
			
			while (nPixel--)
				*pRGB++ = *pGray++;

			::shmdt(pRGB);
			}
		if ((img.dataFormats & kHSV) == kHSV)
			{
			pHSV = (hsv32*)::shmat(img.hsvImage.shMemKey, NULL, 0);
			nPixel = img.width * img.height;
			
			while (nPixel--)
				*pHSV++ = *pGray++;

			::shmdt(pHSV);
			}
		::shmdt(pGray);
		}
	}

void ImageFactory::updateImageIDLFromRGB(ImageIDL& img)
	{
	long	nPixel;
	gray*	pGray;
	rgb24*	pRGB;
	hsv32*	pHSV;

	if (img.rgbImage.sharedMemoryValid && img.rgbImage.dataValid)
		{
		if ((img.dataFormats & kGray) == kGray)
			{
			pRGB = (rgb24*)img.rgbImage.serverShMemData;
			pGray = (gray*)img.grayImage.serverShMemData;
			nPixel = img.width * img.height;
			
			while (nPixel--)
				*pGray++ = *pRGB++;

			img.dataFormats |= kGray;
			}
		if ((img.dataFormats & kHSV) == kHSV)
			{
			pRGB = (rgb24*)img.rgbImage.serverShMemData;
			pHSV = (hsv32*)img.hsvImage.serverShMemData;
			nPixel = img.width * img.height;
			
			while (nPixel--)
				*pHSV++ = *pRGB++;

			img.dataFormats |= kHSV;
			}
		}
	}

void ImageFactory::updateImageIDLFromHSV(ImageIDL& img)
	{
	long	nPixel;
	gray*	pGray;
	rgb24*	pRGB;
	hsv32*	pHSV;

	if (img.hsvImage.sharedMemoryValid && img.hsvImage.dataValid)
		{
		pHSV = (hsv32*)img.hsvImage.serverShMemData;

		if ((img.dataFormats & kGray) == kGray)
			{
			pGray = (gray*)img.grayImage.serverShMemData;
			nPixel = img.width * img.height;
			
			while (nPixel--)
				*pGray++ = *pHSV++;
			}

		if ((img.dataFormats & kRGB) == kRGB)
			{
			pRGB = (rgb24*)img.rgbImage.serverShMemData;
			nPixel = img.width * img.height;
			
			while (nPixel--)
				*pRGB++ = *pHSV++;
			}
		}
	}

void ImageFactory::allocateImage(ImageIDL& img)
	{
	if ((img.dataFormats & kGray) == kGray)
		allocateData(img.grayImage, img.width, img.height, sizeof(gray));
	if ((img.dataFormats & kRGB) == kRGB)
		allocateData(img.rgbImage, img.width, img.height, sizeof(rgb24));
	if ((img.dataFormats & kHSV) == kHSV)
		allocateData(img.hsvImage, img.width, img.height, sizeof(hsv32));
	}

void ImageFactory::deallocateImage(ImageIDL& img)
	{
	deallocateData(img.grayImage);
	deallocateData(img.rgbImage);
	deallocateData(img.hsvImage);
	}

void ImageFactory::allocateData(SharedImageIDL& img, long w, long h, long s)
	{
	img.shMemKey = ::shmget(key_t(0), w * h * s, IPC_CREAT|IPC_EXCL|0x1ff);
	if (img.shMemKey == -1)
		throw Miro::EDevIO();
	
	img.serverShMemData = (long)::shmat(img.shMemKey, NULL, 0);
	if (img.serverShMemData == -1)
		throw Miro::EDevIO();

	img.sharedMemoryValid = true;
	}

void ImageFactory::deallocateData(SharedImageIDL& img)
	{
	img.dataValid = false;

	struct shmid_ds	buf;

	if (img.sharedMemoryValid)
		{
		::shmdt((void*)img.serverShMemData);
		::shmctl(img.shMemKey, IPC_RMID, &buf);
		}

	img.sharedMemoryValid = false;
	}

}
