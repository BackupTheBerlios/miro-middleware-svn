#ifndef C_MIRO_IMAGE_SOURCE
#define C_MIRO_IMAGE_SOURCE

#include "MiroImage.h"
#include "ImageFactory.h"

namespace MiroImage {

class imageSource :public shared
	{
	public:
		imageSource();
		virtual	~imageSource();
		
		virtual void	requestImage(ImageIDL& img, long tolerance) = 0;
		virtual void	releaseImage(ImageIDL& img) = 0;

		virtual void	getImage(ImageIDL& img);
		virtual void	getStereoImage(StereoImageIDL& img);

	protected:	
		virtual void	parameterCheck(ImageIDL& img, long tolerance);
		virtual void	parameterCheck(StereoImageIDL& img, long tolerance);
		
		virtual	booleanType	validChannel(long) const;
		virtual	booleanType	validSubfield(long) const;

		virtual void	getLeftImage(ImageIDL& img) = 0;
		virtual void	getRightImage(ImageIDL& img) = 0;

		integerType	supportedSubfields;
		integerType	supportedChannel;
	};

}
#endif
