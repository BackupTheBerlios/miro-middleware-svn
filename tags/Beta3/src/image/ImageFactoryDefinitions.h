#ifndef C_IMAGE_FACTORY_DEFINITIONS
#define C_IMAGE_FACTORY_DEFINITIONS

#include "MiroImage.h"
#include "idl/ExceptionC.h"
#include "idl/ImageDefinitionsC.h"

using namespace Miro;

namespace MiroImage {

/**
	Data source
*/
enum
	{
	kNoSource = 0,
	kVideoSource = 1,
	kRenderSource = 2,
	kTestPatternSource = 3
	};

/**
	Data source channel
*/
enum
	{
	kNoChannel = 0,				//	no channel
	kMonoChannel = 1,			//	single channel
	kLeftChannel = 1 << 1,		//	left channel
	kRightChannel = 1 << 2,		//	right channel
	kStereo = kLeftChannel|kRightChannel
	};

/**
	Subfields
*/
enum
	{
	kNoSubfield = 0,		//	no subfield
	kEvenSubfield = 1,		//	even subfield
	kOddSubfield = 1 << 1,	//	odd subfield
	kAllSubfields = kEvenSubfield|kOddSubfield
	};

/**
	Image format
*/
enum
	{
	kGray = 1 << 1,
	kGrayFloat = 1 << 4,
	kRGB = 1 << 5,
	kRGBFloat = 1 << 14,
	kHSV = 1 << 15,
	kHSVFloat = 1 << 18
	};
/**
	Parameter tolerance
*/
enum
	{
	kNoTolerance = 0,
	kSizeTolerance = 1,				//	allow best matching size
	kDepthTolerance = 1 << 1,		//	allow best matching depth
	kChannelTolerance = 1 << 2,		//	allow best matching channel
	kSubfieldTolerance = 1 << 3		//	allow best matching subfield
	};

void	initializeSharedImageIDL(SharedImageIDL&);
void	initializeImageIDL(ImageIDL&);
void	initializeStereoImageIDL(StereoImageIDL&);
}
#endif
