/*! \file VideoDevice1394.cpp
 *  \brief Implementation for a firewire video device
 *
 * -----------------------------------------------------
 *
 * RoboCup Middle Size Team, Technische Universitaet Graz
 * "Mostly Harmless" 
 * http://www.robocup.tugraz.at
 *
 * Project: Miro::Video
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 * $Log$
 * Revision 1.2  2003/05/12 16:34:35  hutz
 * bug hunt on fire wire
 *
 * Revision 1.1  2003/05/12 11:48:05  hutz
 * added the fire wire dc support from graz
 * propagated changes and cleanups introduced by fire wire dc
 * cleaned up the parameter handling of fire wire dc
 *
 * Revision 1.8  2003/03/11 18:19:16  muehlenf
 * Added device-parameter.
 *
 * Revision 1.7  2003/03/10 10:25:20  muehlenf
 * Corrected reading of the white-balance.
 *
 * Revision 1.6  2003/03/10 07:40:34  muehlenf
 * Additional parameters for firewire cameras.
 *
 * Revision 1.5  2003/01/20 10:54:17  muehlenf
 * Interface-test for the VideoService (using VideoDeviceDummy).
 *
 * Revision 1.4  2003/01/14 10:19:53  muehlenf
 * Started using configure & automake.
 *
 * Revision 1.3  2003/01/13 00:12:27  muehlenf
 * Some minor modifications.
 *
 * Revision 1.2  2003/01/10 13:46:46  muehlenf
 * Some corrections and tests, first working version.
 *
 * Revision 1.1  2003/01/09 11:46:20  muehlenf
 * Added firewire support to Miro VideoService
 *
 *
 */

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include <ace/OS.h>

#include "VideoDevice1394.h"

#ifdef HAVE_VIDEODEVICE1394

#include "VideoConfig.h"
#include "VideoConverterYUV411toRGB.h"
#include "VideoConverterYUV422toRGB.h"
#include "VideoConverterYUV411toYUV.h"
#include "VideoConverterYUV422toYUV.h"

#include <miro/Exception.h>

#include <iostream>

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

#define NUM_BUFFERS 8
#define DROP_FRAMES 1
#define DEVICE_NAME "/dev/video1394/0"

// #define RAW_RGB

namespace Video
{

//---------------------------------------------------------------
    struct Feature1394
    {
        char const * name;
    	int const * pValue;
        int code;
    	int (* set_function) (raw1394handle_t handle,
			      nodeid_t node,
			      unsigned int sharpness);
    };

//---------------------------------------------------------------
    static Feature1394 feature1394[12] =
    {
    	{ "brightness",  NULL, FEATURE_BRIGHTNESS,  dc1394_set_brightness },
	{ "exposure",    NULL, FEATURE_EXPOSURE,    dc1394_set_exposure },
	{ "focus",       NULL, FEATURE_FOCUS,	    dc1394_set_focus },
	{ "gain",        NULL, FEATURE_GAIN,	    dc1394_set_gain },
	{ "gamma",       NULL, FEATURE_GAMMA,	    dc1394_set_gamma },
	{ "hue",         NULL, FEATURE_HUE,	    dc1394_set_hue },
	{ "iris",        NULL, FEATURE_IRIS,	    dc1394_set_iris },
	{ "saturation",  NULL, FEATURE_SATURATION,  dc1394_set_saturation },
	{ "sharpness",   NULL, FEATURE_SHARPNESS,   dc1394_set_sharpness },
	{ "shutter",     NULL, FEATURE_SHUTTER,     dc1394_set_shutter },
	{ "temperature", NULL, FEATURE_TEMPERATURE, dc1394_set_temperature },
	{ "trigger",     NULL, FEATURE_TRIGGER,     dc1394_set_trigger_mode }
    };

//---------------------------------------------------------------
  struct FeatureTable
  {
    static const unsigned int NUM_FEATURES1394 = 12;

    FeatureTable(const FireWireParameters& _params) : 
      feature_(	feature1394 ) 
    {
      feature_[0].pValue = & (_params.brightness);
      feature_[1].pValue = & (_params.exposure);
      feature_[2].pValue = & (_params.focus);
      feature_[3].pValue = & (_params.gain);
      feature_[4].pValue = & (_params.gamma);
      feature_[5].pValue = & (_params.hue);
      feature_[6].pValue = & (_params.iris);
      feature_[7].pValue = & (_params.saturation);
      feature_[8].pValue = & (_params.sharpness);
      feature_[9].pValue = & (_params.shutter);
      feature_[10].pValue = & (_params.temperature);
      feature_[11].pValue = & (_params.trigger);
    }
    const Feature1394& operator[] (unsigned int _index) { 
      return feature_[_index]; 
    }
    Feature1394 const * begin() const { 
      return &feature_[0]; 
    }
    Feature1394 const * end() const { 
      return &feature_[NUM_FEATURES1394]; 
    }

  protected:
    Feature1394 feature_[NUM_FEATURES1394];
  };


//---------------------------------------------------------------
    VideoDevice1394::VideoDevice1394() :
      converter_(NULL),
      video_fd_(-1),
      device_name_(DEVICE_NAME),
      is_open_(false),
      handle_(0),
      p_camera_(0),
      frame_buffers_(0),
      frame_rate_(FRAMERATE_30)
    {
        DBG(std::cout << "Constructing VideoDevice1394." << std::endl);
	p_camera_ = new dc1394_cameracapture;
    }
    
//---------------------------------------------------------------
    VideoDevice1394::~VideoDevice1394()
    {
        handleDisconnect();
	delete p_camera_;
	delete converter_;
    } 

//---------------------------------------------------------------
    void * VideoDevice1394::grabImage(ACE_Time_Value& _stamp) const
    {
      DBG(std::cout << "VideoDevice1394: grabImage" << std::endl);
	
	dc1394_dma_single_capture(p_camera_);
	_stamp = ACE_OS::gettimeofday();

	int next_buffer = (iCurrentBuffer + 1)%iNBuffers;
	void * actual_start = frame_buffers_ + next_buffer*getImageSize();
	if (converter_)
	    (*converter_)((const unsigned char *)p_camera_->capture_buffer,
	                  (unsigned char *)actual_start,
			  imgWidth*imgHeight);
	else
            memcpy (actual_start, p_camera_->capture_buffer, getImageSize());
	iCurrentBuffer = next_buffer;
	
	dc1394_dma_done_with_buffer(p_camera_);
	
        return actual_start;
    }
    
//---------------------------------------------------------------
    void VideoDevice1394::handleConnect(const int fd, const Parameters& params)
    {
	DBG(std::cout << "Connecting VideoDevice1394." << std::endl);
	
	video_fd_ = fd;
	initDevice();
	initSettings(params_->fireWire);
	
	setFormat(::Video::getFormat(params.format));
	setSource(::Video::getSource(params.source));
	setPalette(::Video::getPalette(params.palette));
	setSubfield(::Video::getSubfield(params.subfield));
	setSize(params.width, params.height);
	
	initBuffers(params.buffers);
	initCapture();
    }

//---------------------------------------------------------------
    void VideoDevice1394::handleDisconnect()
    {
	DBG(std::cout << "VideoDevice1394:handleDisconnect: frames captured " << iNFramesCaptured << std::endl);
	
        if (is_open_)
	{
	    DBG(std::cout << "VideoDevice1394::handleDisconnect: close" << std::endl);
            dc1394_stop_iso_transmission(handle_, p_camera_->node);
	    dc1394_dma_unlisten(handle_, p_camera_);
	    dc1394_dma_release_camera(handle_, p_camera_);
	    is_open_ = false;
	}

	cleanupDevice();
	video_fd_ = -1;
	
	if (frame_buffers_)
	{
	    delete frame_buffers_;
	    frame_buffers_ = 0;
	}
    }
    
//---------------------------------------------------------------
    void VideoDevice1394::initDevice(int port)
    {
	DBG(std::cout << "VideoDevice1394::initDevice " << port << std::endl);

        handle_ = dc1394_create_handle(port);
	if (!handle_)
	{
	    DBG(std::cout << "VideoDevice1394::initDevice: unable to create handle" << std::endl);
	    throw Miro::Exception("VideoDevice1394::initDevice: unable to create handle");
	}

        int num_nodes = raw1394_get_nodecount(handle_);
        int camera_count = 0;
        nodeid_t * camera_nodes = dc1394_get_camera_nodes(handle_, &camera_count, 1);
        DBG(std::cout << "VideoDevice1394::initDevice: cameracount = " << camera_count << std::endl);
	if (camera_count < 1)
	        throw Miro::Exception("VideoDevice1394::initDevice: no camera found");

	p_camera_->node = camera_nodes[0];
	DBG(std::cout << "VideoDevice1394::initDevice: node = " << p_camera_->node << "/" << num_nodes << std::endl);
	if (p_camera_->node == num_nodes - 1)
	    throw Miro::Exception("VideoDevice1394::initDevice: camera is highest numbered node.");

        dc1394_stop_iso_transmission(handle_, p_camera_->node);

	if (dc1394_get_camera_feature_set(handle_, p_camera_->node, &features_) != DC1394_SUCCESS)
	    throw Miro::Exception("VideoDevice1394::initDevice: unable to get camera features");
	
	DBG(dc1394_print_feature_set(&features_));
    }

//---------------------------------------------------------------
    void VideoDevice1394::cleanupDevice()
    {
	DBG(std::cout << "VideoDevice1394::cleanupDevice " << std::endl);

	if (handle_)
	{
            dc1394_destroy_handle(handle_);
	    handle_ = 0;
	}
    }

//---------------------------------------------------------------
    void VideoDevice1394::setFormat(VideoFormat fmt)
    {
        DBG(std::cout << "VideoDevice1394::setFormat" << std::endl);
	formatID = fmt;
    }

//---------------------------------------------------------------
    void VideoDevice1394::setSource(VideoSource src)
    {
        DBG(std::cout << "VideoDevice1394::setSource" << std::endl);
	if (src != source1394)
	   throw Miro::Exception("VideoDevice1394::setSource: unsupported source requested");
	sourceID = src;
    }

//---------------------------------------------------------------
    void VideoDevice1394::setPalette(VideoPalette pal)
    {
        DBG(std::cout << "VideoDevice1394::setPalette" << std::endl);
	requestedPaletteID = pal;
	if (pal == paletteBGR)
	    devicePaletteID = paletteRGB;
	else
	    devicePaletteID = pal;
	if (devicePaletteID != paletteRGB && devicePaletteID != paletteYUV)
	   throw Miro::Exception("VideoDevice1394::setPalette: unsupported palette requested");
    }

//---------------------------------------------------------------
    void VideoDevice1394::setSubfield(VideoSubfield sub)
    {
        DBG(std::cout << "VideoDevice1394::setSubfield" << std::endl);
	requestedSubfieldID = sub;
	if (sub != subfieldAll)
	    throw Miro::Exception("VideoDevice1394::setSubfield: subfields not supported yet");
	deviceSubfieldID = subfieldAll;
    }

//---------------------------------------------------------------
    void VideoDevice1394::setSize(int w, int h)
    {
        DBG(std::cout << "VideoDevice1394::setSize" << std::endl);
	imgWidth = w;
	imgHeight = h;
	
	if (converter_) delete converter_;
	converter_ = 0;

#ifdef RAW_RGB
        if (w != 640 || h != 480 || devicePaletteID != paletteRGB)
            throw Miro::Exception("VideoDevice1394::setSize: RGB supports only 640x480");
	image_format_ = MODE_640x480_RGB;
	if (frame_rate_ == FRAMERATE_30) frame_rate_ = FRAMERATE_15;
#else
        if (w == 640 && h == 480)
	{
            image_format_ = MODE_640x480_YUV411;
	    if (devicePaletteID == paletteRGB)
                converter_ = new VideoConverterYUV411toRGB;
	    else	 
                converter_ = new VideoConverterYUV411toYUV;
	}
	else if (w == 320 && h == 240)
	{
            image_format_ = MODE_320x240_YUV422;
	    if (devicePaletteID == paletteRGB)
                converter_ = new VideoConverterYUV422toRGB;
	    else
                converter_ = new VideoConverterYUV422toYUV;
	}
        else
            throw Miro::Exception("VideoDevice1394::setSize: unsupported image size");
#endif
    }

//---------------------------------------------------------------
    void VideoDevice1394::initBuffers(int buf_cnt)
    {
        if (frame_buffers_) delete frame_buffers_;
	iNBuffers = buf_cnt;
	iCurrentBuffer = iNBuffers-1;
	iNFramesCaptured = 0;
        frame_buffers_ = new char [getImageSize()*iNBuffers];
    }

//---------------------------------------------------------------
    void VideoDevice1394::initCapture()
    {
	unsigned int channel;
	unsigned int speed;
	if (dc1394_get_iso_channel_and_speed(handle_, p_camera_->node, &channel, &speed) != DC1394_SUCCESS)
	    throw Miro::Exception("VideoDevice1394:initDevice: unable to get iso channel number");
	std::cout << "ISO channel: " << channel << std::endl;
	std::cout << "    speed: " << speed << std::endl;
	channel = 1;

#ifdef RAW_RGB		
	if (dc1394_dma_setup_capture(handle_,
	                             p_camera_->node,
				     channel,
				     FORMAT_VGA_NONCOMPRESSED,
				     image_format_,
				     SPEED_400,
				     frame_rate_,
				     NUM_BUFFERS,
				     DROP_FRAMES,
				     device_name_.c_str(),
				     p_camera_) != DC1394_SUCCESS)
	    throw Miro::Exception("VideoDevice1394::handleConnect: unable to setup camera (RAW_RGB)");
#else
	if (dc1394_dma_setup_capture(handle_,
	                             p_camera_->node,
				     channel,
				     FORMAT_VGA_NONCOMPRESSED,
				     image_format_,
				     SPEED_400,
				     frame_rate_,
				     NUM_BUFFERS,
				     DROP_FRAMES,
				     device_name_.c_str(),
				     p_camera_) != DC1394_SUCCESS)
	    throw Miro::Exception("VideoDevice1394::handleConnect: unable to setup camera");
#endif
	
	if (dc1394_start_iso_transmission(handle_, p_camera_->node) != DC1394_SUCCESS)
	    throw Miro::Exception("VideoDevice1394::handleConnect: unable to start camera iso transmission");
	is_open_ = true;
    }

//---------------------------------------------------------------
    void VideoDevice1394::initSettings(const FireWireParameters & _params)
    {

      // set standartized features
      FeatureTable featureTable(_params);
      Feature1394 const * first;
      Feature1394 const * last = featureTable.end();
      for (first = featureTable.begin(); first != last; ++first) {
	if (*(first->pValue) == -1) {
	  dc1394_auto_on_off(handle_, p_camera_->node, first->code, 1);
	}
	else {
	  if (first->set_function(handle_,
				  p_camera_->node,
				  (*first->pValue)) != DC1394_SUCCESS) {
	    std::cout << "WARNING: Feature "
		      << first->name << " = " << (*first->pValue)
		      << " not set." << std::endl;
	  }
	  dc1394_auto_on_off(handle_, p_camera_->node, first->code, 0);
	}
      }

      // white balance
      if (_params.whiteBalance0 == -1 ||
	  _params.whiteBalance1 == -1) {
	dc1394_auto_on_off(handle_, p_camera_->node, FEATURE_WHITE_BALANCE, 1);
      }
      else {
	if (dc1394_set_white_balance(handle_,
				     p_camera_->node,
				     _params.whiteBalance0,
				     _params.whiteBalance1) != DC1394_SUCCESS) {
	  std::cout << "WARNING: Feature "
		    << "WhiteBalance = " 
		    << _params.whiteBalance0 << ":" << _params.whiteBalance1
		    << " not set." << std::endl;
	}
	dc1394_auto_on_off(handle_, p_camera_->node, FEATURE_WHITE_BALANCE, 0);
      }

      // set framerate
      int value = _params.framerate;
      if (value <= 1)	   
	frame_rate_ = FRAMERATE_1_875;
      else if (value <= 3)
	frame_rate_ = FRAMERATE_3_75;
      else if (value <= 7)
	frame_rate_ = FRAMERATE_7_5;
      else if (value <= 15)
	frame_rate_ = FRAMERATE_15;
      else
	frame_rate_ = FRAMERATE_30;
    }
};

#endif // HAVE_VIDEODEVICE1394

