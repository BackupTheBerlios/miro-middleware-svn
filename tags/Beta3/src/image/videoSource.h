#ifndef C_VIDEO_SOURCE
#define C_VIDEO_SOURCE

#ifdef macintosh
#define VIDEO_METEOR
#endif

#include "imageSource.h"
#ifdef VIDEO_V4L
	#include "frameGrabberV4L.h"
#endif
#ifdef VIDEO_METEOR
	#include "frameGrabberMeteor.h"
#endif

namespace MiroImage {

class videoSource :public imageSource
	{
	public:
		videoSource(const char*, const char*, bool force = true);
		virtual	~videoSource();
		
		virtual void	requestImage(ImageIDL& img, long tolerance);
		virtual void	releaseImage(ImageIDL& img);

		booleanType		isLeftOpen() const;
		booleanType		isRightOpen() const;

	protected:	
		virtual void	getLeftImage(ImageIDL& img);
		virtual void	getRightImage(ImageIDL& img);

	private:
	#ifdef VIDEO_V4L
		frameGrabberV4L		grabberLeft;
		frameGrabberV4L		grabberRight;
	#endif
	#ifdef VIDEO_METEOR
		frameGrabberMeteor	grabberLeft;
		frameGrabberMeteor	grabberRight;
	#endif
	};

}
#endif
