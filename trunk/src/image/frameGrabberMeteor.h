#ifndef C_FRAME_GRABBER_METEOR
#define C_FRAME_GRABBER_METEOR

#include "ioctl_meteor.h"
#include "frameGrabber.h"

namespace MiroImage {

#define MAX_CONCURRENT_METEOR_GRABBERS 2

class frameGrabberMeteor :public frameGrabber
	{
	public:
		frameGrabberMeteor(integerType buffers = 8);
		virtual	~frameGrabberMeteor();
		
		virtual void	requestImage(ImageIDL&, long tolerance);
		virtual void	releaseImage(ImageIDL& img);
	
	protected:
		booleanType		imageAvailable();

		virtual void	grabImage(ImageIDL& img);

		virtual void	copyRGB(ImageIDL& img);
		virtual void	copyGray(ImageIDL& img);
	
	private:
		friend void usr1SignalHandler(int);
		friend void usr2SignalHandler(int);

		void			handleSignal();

		integerType		getCurrentErrorCount();
		void			freeOldFrames();

		static	integerType	staticMeteorGrabberCount;
		static	frameGrabberMeteor*	frameGrabberMeteorSelf[MAX_CONCURRENT_METEOR_GRABBERS];

		integerType		iGrabberIndex;
		booleanType		bMeteorContinous;

		void*			pCurrentFrameMappedMemory;

		integerType		iCurrentFrameTotal;
		integerType		iCurrentFrameSave;
		integerType		iCurrentFrame;
		integerType		iNextFrame;
		integerType		iNFrames;
		booleanType		bFrameIsAvailable;
		integerType		iFrameSize;
		
	#ifdef MIRO
		Miro::TimeIDL*	timestamps;
	#else
		uint32*			timestamps;
	#endif

		meteor_mem*		meteorMemory;
		meteor_geomet	meteorGeometry;
		int				meteorSource;
		int				meteorInputFormat;
	};

}
#endif
