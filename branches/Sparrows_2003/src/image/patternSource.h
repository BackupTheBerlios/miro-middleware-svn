#ifndef C_MIRO_PATTERN_SOURCE
#define C_MIRO_PATTERN_SOURCE

#include "Pixels.h"
#include "imageSource.h"

namespace MiroImage {

class patternSource :public imageSource
	{
	public:
		patternSource();
		virtual	~patternSource();
		
		virtual void	requestImage(ImageIDL& img, long tolerance);
		virtual void	releaseImage(ImageIDL& img);

	protected:	
		virtual void	getLeftImage(ImageIDL& img);
		virtual void	getRightImage(ImageIDL& img);
	
	private:
		ImageIDL		leftMapStruct;
		ImageIDL		rightMapStruct;

		rgb24*			leftMap;
		rgb24*			rightMap;
		
		integerType		iNRequests;
		integerType		iLeftOffset;
		integerType		iRightOffset;
	};

}
#endif
