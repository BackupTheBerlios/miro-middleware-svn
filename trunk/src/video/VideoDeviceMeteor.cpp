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
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <miro/Exception.h>
#include <ace/OS.h>

#include "VideoDeviceMeteor.h"
#include "BufferManagerMeteor.h"

#include "miro/VideoHelper.h"

#undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Video
{
  using std::cout;
  using std::cerr;
  using std::endl;

  //--------------------------------------------------------------------------
  // Hardware specifica
  //--------------------------------------------------------------------------

  DeviceMeteor::DeviceMeteor(const Miro::ImageFormatIDL& _inputFormat) :
    Super(_inputFormat),
    devName_(),
    ioBuffer_(),
    connector_()
  {
    formatLookup[FORMAT_AUTO]  = METEOR_FMT_AUTOMODE;
    formatLookup[FORMAT_PAL]   = METEOR_FMT_PAL;
    formatLookup[FORMAT_NTSC]  = METEOR_FMT_NTSC;
    formatLookup[FORMAT_SECAM] = METEOR_FMT_SECAM;

    paletteLookup[Miro::GREY_8] = METEOR_GEO_YUV_PLANAR;
    paletteLookup[Miro::RGB_24] = METEOR_GEO_RGB24;
    paletteLookup[Miro::RGB_32] = METEOR_GEO_RGB24;
    paletteLookup[Miro::BGR_24] = METEOR_GEO_RGB24;
    paletteLookup[Miro::BGR_32] = METEOR_GEO_RGB24;

    sourceLookup[SOURCE_COMPOSITE1] = METEOR_INPUT_DEV1;
    sourceLookup[SOURCE_COMPOSITE2] = METEOR_INPUT_DEV2;
    sourceLookup[SOURCE_COMPOSITE3] = METEOR_INPUT_DEV3;
    sourceLookup[SOURCE_SVIDEO] =     METEOR_INPUT_DEV_SVIDEO;
  }

  DeviceMeteor::~DeviceMeteor()
  {
  }

  BufferManager * 
  DeviceMeteor::bufferManagerInstance() const 
  {
    return new BufferManagerMeteor(ioBuffer_.get_handle(),
				   Miro::getImageSize(outputFormat_), 
				   buffer_);
  }

  void
  DeviceMeteor::init(Miro::Server& _server, FilterParameters const * _params)
  {
    DBG(cout << "Connecting DeviceMeteor." << endl);

    params_ = dynamic_cast<AVDeviceParameters const *>(_params);
    assert(params_ != NULL);

    devName_.set(params_->device.c_str());
    if (connector_.connect(ioBuffer_, 
			  devName_, 
			  0, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      cerr << "Failed to open device: " << params_->device << endl
	   << "Propably running on the wrong machine?" << endl;
      throw Miro::CException(errno, std::strerror(errno));
    }

    fcntl(ioBuffer_.get_handle(), F_SETFD, FD_CLOEXEC);

    setPalette();
    setSize();

    meteorGeometry.frames = 1;
    if (::ioctl(ioBuffer_.get_handle(), METEORSETGEO, &meteorGeometry) < 0)
      throw Miro::Exception("METEORSETGEO");

    setSource();
    setFormat();

    buffer_ = (unsigned char*)mmap(0, 
				   Miro::getImageSize(outputFormat_),
				   PROT_READ, MAP_SHARED,
				   ioBuffer_.get_handle(),
				   0);
    if ((int)buffer_ == -1)
      throw Miro::CException(errno, "mmap()");

    Super::init(_server, _params);
  }

  void
  DeviceMeteor::fini()
  {
    DBG(cout << "DeviceBTTV." << endl);

    if ((int)buffer_ != -1 && buffer_ != NULL) {
      munmap(buffer_, Miro::getImageSize(outputFormat_));
      buffer_ = NULL;
    }
    ioBuffer_.close();

    // removing buffer ptr
    buffer_ = NULL;
  }

  void 
  DeviceMeteor::setFormat()
  {
    VideoFormat id = getFormat(params_->format);
    
    if (formatLookup[id] != -1)
    {
      if (::ioctl(ioBuffer_.get_handle(), METEORSFMT, &formatLookup[id]) < 0)
	throw Miro::Exception("METEORSFMT");
    }
    else
      throw Miro::Exception();
  }

  void 
  DeviceMeteor::setSource()
  {
    DBG(cout << "DeviceMeteor: setSource" << endl);

    VideoSource id = getSource(params_->source);
    if (sourceLookup[id] == -1)
      throw Miro::Exception(std::string("DeviceMeteor::setSource - Unsupported source: ") +
			    params_->source);
    
    int	err = ioctl(ioBuffer_.get_handle(), METEORSINPUT, &sourceLookup[id]);
    if (err == -1)
      throw Miro::CException(errno, "METEORSINPUT");
  }
  
  void
  DeviceMeteor::setPalette()
  {
    if (inputFormat_.palette == Miro::RGB_24)
      outputFormat_.palette =  Miro::RGB_32;
    
    if (paletteLookup[outputFormat_.palette] == -1)
      throw Miro::Exception("DeviceMeteor::setPalette - Unsupported palette. ");
      
    meteorGeometry.oformat = METEOR_GEO_YUV_PLANAR;
  }
  
  void
  DeviceMeteor::setSize()
  {
    meteorGeometry.rows =  outputFormat_.width;
    meteorGeometry.columns = outputFormat_.height;
  }
}

