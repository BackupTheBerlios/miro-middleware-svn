#ifndef C_FRAME_GRABBER
#define C_FRAME_GRABBER

#include "ImageFactoryDefinitions.h"

namespace MiroImage {

const	integerType	iMaxPALColumns = 768;
const	integerType	iMaxPALRows = 576;

class frameGrabber
	{
	public:
		frameGrabber();
		virtual	~frameGrabber();

		virtual void	open(const char*);
		virtual void	close();

		virtual void	requestImage(ImageIDL&, long tolerance) = 0;
		virtual void	releaseImage(ImageIDL& img);

		virtual void	getImage(ImageIDL&);

		virtual bool	isOpen() const;
		virtual bool	isMapped() const;
		virtual bool	isRequested() const;
	
	protected:
		virtual void	modeCheck(ImageIDL& img);
		virtual void	parameterCheck(ImageIDL& img, long tolerance);

		virtual void	grabImage(ImageIDL& img) = 0;

		virtual void	copyRGB(ImageIDL& img) = 0;
		virtual void	copyGray(ImageIDL& img) = 0;
		virtual void	map(integerType s, booleanType rw = false);
		virtual void	unmap();
		
		std::string		deviceName;
		integerType	iNCaptureRetries;
		
		integerType	iNRequests;
		integerType	iCurrentColumns;
		integerType	iCurrentRows;
		integerType	iCurrentFormat;
		integerType	iCurrentPixelSize;
		
		integerType	iMinColumns;
		integerType	iMaxColumns;
		
		integerType	iMinRows;
		integerType	iMaxRows;
		
		int			deviceFd;
		void*		pMappedMemory;
		integerType	iMappedMemorySize;
	};

}
#endif
