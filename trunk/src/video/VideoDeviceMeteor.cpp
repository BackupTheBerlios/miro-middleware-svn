// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
#include "miro/Log.h"

namespace Video
{
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
    return new BufferManagerMeteor(this,
				   ioBuffer_.get_handle(),
				   Miro::getImageSize(outputFormat_), 
				   buffer_);
  }

  void
  DeviceMeteor::init(Miro::Server& _server, FilterParameters const * _params)
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceMeteor: Connecting DeviceMeteor.");

    params_ = dynamic_cast<AVDeviceParameters const *>(_params);
    assert(params_ != NULL);

    devName_.set(params_->device.c_str());
    if (connector_.connect(ioBuffer_, 
			  devName_, 
			  0, ACE_Addr::sap_any, 0, O_RDWR) == -1) {
      MIRO_DBG_OSTR(VIDEO,
		    LL_ERROR, 
		    "Video::DeviceMeteor: Failed to open device: " << params_->device <<
		    "\nPropably running on the wrong machine?");
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
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceMeteor::fini()");

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
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::DeviceMeteor::setSource()");

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

