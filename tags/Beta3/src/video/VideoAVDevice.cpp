#include "VideoAVDevice.h"

#include "miro/Exception.h"

#include <string>

namespace
{
  struct FormatPair
  {
    std::string first;
    Video::AVDevice::VideoFormat second;
  };

  FormatPair format[Video::AVDevice::NUM_FORMAT_ENTRIES] = {
    { "auto", Video::AVDevice::FORMAT_AUTO },
    { "pal", Video::AVDevice::FORMAT_PAL },
    { "ntsc", Video::AVDevice::FORMAT_NTSC },
    { "secam", Video::AVDevice::FORMAT_SECAM }
  };

  struct SourcePair
  {
    std::string first;
    Video::AVDevice::VideoSource second;
  };

  SourcePair source[Video::AVDevice::NUM_SOURCE_ENTRIES] = {
    { "composite1", Video::AVDevice::SOURCE_COMPOSITE1 },
    { "composite2", Video::AVDevice::SOURCE_COMPOSITE2 },
    { "composite3", Video::AVDevice::SOURCE_COMPOSITE3 },
    { "svideo", Video::AVDevice::SOURCE_SVIDEO },
    { "tuner", Video::AVDevice::SOURCE_TUNER }
  };
};

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(AVDevice);


  AVDevice::AVDevice(Miro::ImageFormatIDL const& _inputFormat) :
    Super(_inputFormat)
  {
    // clear table of supported formats
    for (unsigned int i = 0; i < NUM_FORMAT_ENTRIES; ++i)
      formatLookup[i] = -1;

    // clear table of supported sources
    for (unsigned int i = 0; i < NUM_SOURCE_ENTRIES; ++i)
      sourceLookup[i] = -1;
  }

  AVDevice::VideoFormat 
  AVDevice::getFormat(std::string const & _fmt)
  {
    for (unsigned int i = 0; i < NUM_FORMAT_ENTRIES; ++i) {
      if (format[i].first == _fmt) {
	return format[i].second;
      }
    }

    throw Miro::Exception(std::string("Video::Parameters: unknown format: ") + _fmt);
  }
  
  AVDevice::VideoSource 
  AVDevice::getSource(const std::string & _src)
  {
    for (unsigned int i = 0; i < NUM_SOURCE_ENTRIES; ++i) {
      if (source[i].first == _src) {
	return source[i].second;
      }
    }

    throw Miro::Exception(std::string("Video::Parameters: unknown source: ") + _src);
  }
    

};
