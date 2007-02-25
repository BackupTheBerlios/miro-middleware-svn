// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005
// Department of Software Technology, Technical University of Graz, Austria
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
#ifndef VIDEODEVICE1394_H
#define VIDEODEVICE1394_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "VideoDevice.h"
#include "idl/VideoControlC.h"

#include <libraw1394/raw1394.h>
#if MIRO_HAS_LIBDC1394_VERSION == 1 || MIRO_HAS_LIBDC1394_VERSION == 2
#include <libdc1394/dc1394_control.h>
#else
#include <dc1394/dc1394_control.h>
#endif


namespace Video
{
  class ControlImpl;

  //! Firewire camera support.
  class Device1394 : public Device
  {
    typedef Device Super;
    
  public:
    Device1394(const Miro::ImageFormatIDL& _inputFormat);
    virtual ~Device1394();
	
    FILTER_PARAMETERS_FACTORY(Device1394);

    bool setFeatures(const FeatureSet & features);
    bool getFeatures(FeatureSet & features) const;

  protected:
    virtual BufferManager * bufferManagerInstance() const;
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void fini();
    
    //! Detect and initialize the camera.
    void initDevice(int port = 0);
    //! Close device-driver handles.
    void cleanupDevice();
	
    void setImageFormat();

    //! set extra (and device specific) camera parameters.
    void initSettings();
	
    //! Initialize dma transmission
    void initCapture();

    Device1394Parameters    params_;

  private:
    //! Flag to indicate that the device is initialized.
    bool                    is_open_;
    //! Real device handle.
    raw1394handle_t         handle_;
#if MIRO_HAS_LIBDC1394_VERSION == 1 || MIRO_HAS_LIBDC1394_VERSION == 2
    //! Actual camera features.
    dc1394_feature_set      features_;
    //! Camera capture data structure.
    dc1394_cameracapture *  p_camera_;
    //! Selected image format.
    long                    imageFormat_;
    //! Framerate
    int                     frameRate_;
#else
    //! Actual camera features.
    dc1394featureset_t features_;
    //! Camera capture data structure.
    dc1394camera_t * p_camera_;
    //! Selected image format.
    dc1394video_mode_t      imageFormat_;
    //! Framerate
    dc1394framerate_t       frameRate_;
#endif

    //! Pointer to Control-Interface.
    ControlImpl *           control_;
  };
};
#endif // VIDEODEVICE1394_H

