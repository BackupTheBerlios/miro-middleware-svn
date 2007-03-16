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

#ifndef VIDEODEVICEUNICAP_H
#define VIDEODEVICEUNICAP_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "VideoDevice.h"
#include "idl/VideoControlS.h"
#include "miro/Enumeration.h"

#include <unicap.h>
#include <unicap_status.h>


namespace Video
{
  //! Firewire camera support based on the unicap package.
  class DeviceUnicap : public Device, public virtual POA_Video::CameraControl
  {
    typedef Device Super;
    
  public:
    DeviceUnicap(const Miro::ImageFormatIDL& _inputFormat);
    virtual ~DeviceUnicap();
	
    FILTER_PARAMETERS_FACTORY(DeviceUnicap);

    virtual void getFeature(CameraFeature feature, FeatureSet_out set)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ));
    virtual void setFeature(CameraFeature feature, const FeatureSet & set)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ));
    virtual void getFeatureDescription (FeatureSetVector_out features)
      ACE_THROW_SPEC (( CORBA::SystemException, ::Miro::EOutOfBounds ));

  protected:
    virtual BufferManager * bufferManagerInstance() const;
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void fini();

  private:
    void setupParameters();
    void setupParametersRange(std::string _property, double _value);
    void setupParametersMenu(std::string _property, Miro::Enumeration _enum);
    int getProperty(const std::string _property) const;

    unicap_handle_t handle_;
    unicap_device_t device_;
    unicap_format_t format_;

    static const int MAX_PROPERTIES_ = 64; // max number of possible properties
    unicap_property_t properties_[MAX_PROPERTIES_];
    int propertiesCount_;

    DeviceUnicapParameters params_;
  };
};

#endif

