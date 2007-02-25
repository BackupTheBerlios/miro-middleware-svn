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
#ifndef VideoDeviceDummy_h
#define VideoDeviceDummy_h

#include "VideoDevice.h"

#include <vector>
#include <string>

namespace Video
{
  /// Dummy video device that reads images from disk.
  /**
   * Image or a directory to look for images can be specifed.
   */
  class DeviceDummy : public Device
  {
    typedef Device Super;

  public:
    DeviceDummy(const Miro::ImageFormatIDL& _inputFormatParameters);
    virtual ~DeviceDummy();
	    	    
    FILTER_PARAMETERS_FACTORY(DeviceDummy);

  protected:
    virtual void init(Miro::Server& _server, FilterParameters const * _params);
    virtual void process();

    //--------------------------------------------------------------------------
    // protected data

    DeviceDummyParameters const * params_;

    unsigned int counter_;
    std::vector<std::string> files_;
  };
}

#endif // VideoDeviceDummy_h
