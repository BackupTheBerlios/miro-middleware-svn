// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <ace/OS.h>

#include "VideoDeviceBTTV.h"
#include "BufferManagerBTTV.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include <string>

namespace 
{
  struct SubfieldPair
  {
    std::string first;
    Video::DeviceBTTV::VideoSubfield second;
  };

  SubfieldPair subfield[Video::DeviceBTTV::NUM_SUBFIELD_ENTRIES] = {
    { "all", Video::DeviceBTTV::SUBFIELD_ALL },
    { "odd", Video::DeviceBTTV:: SUBFIELD_ODD},
    { "even", Video::DeviceBTTV:: SUBFIELD_EVEN}
  };
}

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(DeviceBTTV);


  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------
  int DeviceBTTV::gb_pal[64];

  DeviceBTTV::DeviceBTTV(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    devName_(),
    ioBuffer_(),
    connector_(),
    channels(NULL),
    map_(NULL),
    currentBuffer_(0),
    nextBuffer_(0)
  {
    MIRO_LOG_CTOR("Video::DeviceBTTV");

    /**
       cleanup supported format lookup table
    */
    for (int i = 0; i < 64; ++i)
      gb_pal[i] = 0;

    formatLookup[FORMAT_AUTO] = VIDEO_MODE_AUTO;
    formatLookup[FORMAT_PAL] = VIDEO_MODE_PAL;
    formatLookup[FORMAT_NTSC] = VIDEO_MODE_NTSC;
    formatLookup[FORMAT_SECAM] = VIDEO_MODE_SECAM;

    paletteLookup[Miro::GREY_8] = VIDEO_PALETTE_GREY;
    paletteLookup[Miro::RGB_24] = VIDEO_PALETTE_RGB24;
    paletteLookup[Miro::RGB_32] = VIDEO_PALETTE_RGB32;
    paletteLookup[Miro::BGR_24] = VIDEO_PALETTE_RGB24;
    paletteLookup[Miro::BGR_32] = VIDEO_PALETTE_RGB32;

    sourceLookup[SOURCE_COMPOSITE1] = 1;
    sourceLookup[SOURCE_COMPOSITE2] = 3;
    sourceLookup[SOURCE_SVIDEO] = 2;
    sourceLookup[SOURCE_TUNER] = 0;
  }

  DeviceBTTV::~DeviceBTTV()
  {
    MIRO_LOG_DTOR("Video::DeviceBTTV");
  }

  BufferManager * 
  DeviceBTTV::bufferManagerInstance() const 
  {
    unsigned char * bufferAddr[gb_buffers.frames];

    for (int i = 0; i < gb_buffers.frames; ++i)
      bufferAddr[i] = map_ + gb_buffers.offsets[i];

    return new BufferManagerBTTV(this,
				 ioBuffer_.get_handle(),
				 outputFormat_.width,
				 outputFormat_.height,
				 paletteLookup[outputFormat_.palette],
				 gb_buffers.frames, 
				 bufferAddr);
  }

  void 
  DeviceBTTV::init(Miro::Server& _server, FilterParameters const * _params)
  {
    int	err = 0;

    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceBTTV: Connecting DeviceBTTV.");

    params_ = dynamic_cast<DeviceBTTVParameters const *>(_params);
    assert(params_ != NULL);

    devName_.set(params_->device.c_str());
    if (connector_.connect(ioBuffer_, 
			  devName_, 
			  0, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      MIRO_LOG_OSTR(LL_ERROR,
		    "Failed to open device: " << params_->device <<
		    "\nPropably running on the wrong machine?");
      throw Miro::CException(errno, std::strerror(errno));
    }

    fcntl(ioBuffer_.get_handle(), F_SETFD, FD_CLOEXEC);

    /* video capabilities */
    getCapabilities();

    /* video channels aka inputs */
    getChannels();

    if (capability.type & VID_TYPE_CAPTURE) {
      err = ioctl(ioBuffer_.get_handle(), VIDIOCGMBUF, &gb_buffers);
      if (err == -1)
	throw Miro::CException(errno, "DeviceBTTV::handleConnect() - VIDIOCGMBUF");
      map_ = (unsigned char*)mmap(0, gb_buffers.size, PROT_READ, MAP_SHARED, ioBuffer_.get_handle(), 0);
      if ((int)map_ == -1)
	throw Miro::CException(errno, "DeviceBTTV::handleConnect() - mmap()");
    }

    iNBuffers = gb_buffers.frames;

    MIRO_DBG_OSTR(VIDEO, LL_DEBUG, "Video::DeviceBTTV: buffersize: " << gb_buffers.size <<
		  "\nbuffersize/frame: " << gb_buffers.size/gb_buffers.frames <<
		  "\nframes: " << gb_buffers.frames);

    if (gb_buffers.frames < (int)params_->buffers) {
      throw Miro::Exception("Number of requested buffers in the configuration\n" \
			    "exceeds the number of frames available by the device.");
    }

    if (gb_buffers.frames < 2) {
      throw Miro::Exception("Only one frame available by the video device.\n" \
			    "At least two are needed.");
    }

    if (params_->buffers < 2) {
      throw Miro::Exception("Only one buffer available by the configuration.\n" \
			    "At least two are needed.");
    }

    probeAllFormats();

    setFormat();
    setSource();
    setPalette();
    setSize();

    //	preparing buffers

    VideoSubfield subfield = getSubfield(params_->subfield);

    if (subfield != SUBFIELD_ALL) {
      if (capability.type & VID_TYPE_SUBCAPTURE) {

	struct video_capture vc;

	vc.x = 0;
	vc.y = 0;
	vc.width = outputFormat_.width;
	vc.height = outputFormat_.height;
	vc.decimation = 1;
	vc.flags = (subfield == SUBFIELD_ODD)? VIDEO_CAPTURE_ODD : VIDEO_CAPTURE_EVEN;

	err = ioctl(ioBuffer_.get_handle(), VIDIOCSCAPTURE, &vc);
	if (err == -1)
	  throw Miro::CException(errno, "DeviceBTTV::handleConnect() - VIDIOCGMBUF");
      }
      else {
	throw Miro::Exception("Hardware doesn't support subfields.");
      }
    }

    Super::init(_server, _params);
  }

  void DeviceBTTV::fini()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceBTTV::fini()");

    delete[] channels;
    channels = NULL;

    if ((int)map_ != -1 && map_ != NULL) {
      munmap(map_, gb_buffers.size);
      map_ = NULL;
    }
    ioBuffer_.close();
  }


  void DeviceBTTV::setFormat()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceBTTV::setFormat()");
    
    VideoFormat id = getFormat(params_->format);
    if (formatLookup[id] == -1)
      throw Miro::Exception(std::string("DeviceBTTV::setFormat - Unsupported format: ") + 
			    params_->format);
  }

  void DeviceBTTV::setSource()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceBTTV::setSource()");

    VideoSource id = getSource(params_->source);

    if ((sourceLookup[id] != -1) && 
	(sourceLookup[id] < capability.channels)) {
      int err = ioctl(ioBuffer_.get_handle(), VIDIOCSCHAN, &channels[sourceLookup[id]]);
      if (err == -1)
	throw Miro::CException(errno, "DeviceBTTV::setSource() - VIDIOCSCHAN");
    }
    else
      throw Miro::Exception(std::string("DeviceBTTV::setSource - Unsupported source: ") + 
			    params_->source);
  }

  void DeviceBTTV::setPalette()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceBTTV::setPalette()");

    Miro::VideoPaletteIDL id = inputFormat_.palette;

    // as we have intels byte order...
    if (id == Miro::RGB_24)
      id = Miro::BGR_24;
    else if (id == Miro::RGB_32)
      id = Miro::BGR_32;

    if (gb_pal[paletteLookup[id]] == 1)
      outputFormat_.palette = id;
    else
      throw Miro::Exception("DeviceBTTV::setPalette");
  }

  void 
  DeviceBTTV::setSize()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceBTTV::setSize()");

    if (((int)outputFormat_.width < capability.minwidth) ||
	((int)outputFormat_.width > capability.maxwidth))
      throw Miro::Exception("DeviceBTTV::setSize: illegal width");

    if (((int)outputFormat_.height < capability.minheight) ||
	((int)outputFormat_.height > capability.maxheight))
      throw Miro::Exception("DeviceBTTV::setSize: illegal height");
  }

  void DeviceBTTV::getCapabilities()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceBTTV: getCapabilities()");

    int	err = ioctl(ioBuffer_.get_handle(), VIDIOCGCAP, &capability);
    if (err == -1)
      throw Miro::CException(errno, "DeviceBTTV::getCapabilities() - VIDIOCAP");
  }

  void DeviceBTTV::getChannels()
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceBTTV::getChannels()");

    /* input sources */
    channels = new struct video_channel[capability.channels];
    for (int i = 0; i < capability.channels; i++) {
      channels[i].channel = i;
      int err = ioctl(ioBuffer_.get_handle(), VIDIOCGCHAN, &channels[i]);
      if (err == -1)
	throw Miro::CException(errno, "DeviceBTTV::getChannels() - VIDIOCGCHAN");
    }
  }

  bool
  DeviceBTTV::probeFormat(int format)
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceBTTV::probeFormat()");

    struct video_mmap gb;
    int	err;

    if (0 != gb_pal[format])
      goto done;

    gb.frame  = 0;
    gb.width  = 64;
    gb.height = 48;
    gb.format = format;

    err = ioctl(ioBuffer_.get_handle(), VIDIOCMCAPTURE, &gb);
    if (err == -1) {
      gb_pal[format] = 2;
      goto done;
    }

    err = ioctl(ioBuffer_.get_handle(), VIDIOCSYNC, &gb);
    if (err == -1) {
      gb_pal[format] = 3;
      goto done;
    }

    gb_pal[format] = 1;

  done:
    return gb_pal[format] == 1;
  }

  int 
  DeviceBTTV::probeAllFormats()
  {
    int	count = 0;
    for (int i = 0; i < 64; ++i) {
      if (probeFormat(i))
	++count;
    }
    return count;
  }

  DeviceBTTV::VideoSubfield 
  DeviceBTTV::getSubfield(std::string const& _sub)
  {
    for (unsigned int i = 0; i < NUM_SUBFIELD_ENTRIES + 2; ++i) {
      if (_sub == subfield[i].first) {
	return subfield[i].second;
      }
    }

    throw Miro::Exception(std::string("Video::Parameters: unknown subfield: ") + _sub);
  }

};

