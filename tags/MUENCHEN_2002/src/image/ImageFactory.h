#ifndef C_IMAGE_FACTORY
#define C_IMAGE_FACTORY

#include "ImageFactoryDefinitions.h"

namespace MiroImage 
{

class videoSource;
class imageSource;
/**
* Interface to the frame grabber devices.
*/
class ImageFactory 
	{
	public:
		/**
			Constructor
		 */
		ImageFactory();
		/**
			Destructor
		 */
		~ImageFactory();

		/**
		 * Get the current image. This call immediately
		 * returns the last grabbed image.
		 *
		 * @param w Image width (columns) requested.
		 * @param h Image height (rows) requested.
		 * @param formats Image formats requested.
		 * @param tolerance Allow nearest matches for requested parameters.
		 * @param source Data source: Video, test pattern, (maybe sometimes ;-)) 3d-rendering.
		 * @param subfields Subfields requested: odd, even, all.
		 *
		 * @raises Miro::EOutOfBounds, Miro::EDevIO
		 */
		ImageIDL		getImage(long w, long h, long formats,
											long tolerance, long source, long subfields);
		ImageIDL		getLeftImage(long w, long h, long formats,
											long tolerance, long source, long subfields);
		ImageIDL		getRightImage(long w, long h, long formats,
											long tolerance, long source, long subfields);
		StereoImageIDL	getStereoImage(long w, long h, long formats,
											long tolerance, long source, long subfields);

		/**
		 * Get the current image. This call immediately
		 * returns the last grabbed image.
		 *
		 * @param w Image width (columns) of subimage requested.
		 * @param h Image height (rows) of subimage requested.
		 * @param formats Image formats requested.
		 * @param source Data source: Video, test pattern, (maybe sometimes ;-)) 3d-rendering.
		 * @param subfields Subfields requested: odd, even, all.
		 *
		 * @raises Miro::EOutOfBounds, Miro::EDevIO
		 */
		void				requestImage(ImageIDL&);
		ImageIDL		requestImage(long w, long h, long formats,
											long tolerance, long source, long subfields);
		ImageIDL		requestLeftImage(long w, long h, long formats,
											long tolerance, long source, long subfields);
		ImageIDL		requestRightImage(long w, long h, long formats,
											long tolerance, long source, long subfields);
		StereoImageIDL	requestStereoImage(long w, long h, long formats,
											long tolerance, long source, long subfields);

		void			updateImage(ImageIDL&);
		void			updateStereoImage(StereoImageIDL&);

		void			releaseImage(ImageIDL&);
		void			releaseStereoImage(StereoImageIDL&);

		/**
		 * Release any server side data attached to the @ref ImageIDL structure.
		 *
		 * An image not known to the server will raise a OutOfBound exception.
		 *
		 * @param img Image to release.
		 * See the @ref ImageIDL structure for details
		 *
		 * @raises Miro::EOutOfBounds
		 */
		//	void releaseImage(ImageIDL& img);
	
	private:
		void	handleRequestImage(ImageIDL& img, long tolerance);
		void	handleReleaseImage(ImageIDL& img);

		void	videoUpdateImage(ImageIDL& img);
		void	patternUpdateImage(ImageIDL& img);
		void	renderUpdateImage(ImageIDL& img);

		void	updateImageIDL(ImageIDL& image);
		void	updateImageIDLFromGray(ImageIDL& image);
		void	updateImageIDLFromRGB(ImageIDL& image);
		void	updateImageIDLFromHSV(ImageIDL& image);

		void	allocateImage(ImageIDL& image);
		void	deallocateImage(ImageIDL& image);
		
		void	allocateData(SharedImageIDL& img, long w, long h, long s);
		void	deallocateData(SharedImageIDL& img);

		/*
			mapping mono => left or right
		*/
		long			monoChannel;
		imageSource*	video;
		imageSource*	pattern;
		imageSource*	render;
	};
}
#endif
