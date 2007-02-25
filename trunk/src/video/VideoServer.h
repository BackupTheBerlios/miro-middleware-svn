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
#ifndef VideoServer_h
#define VideoServer_h

#include "VideoImpl.h"

#include <ace/Sched_Params.h>

// forward declarations
namespace Miro
{
  class Server;
  class ConfigDocument;
  class ImageFormatIDL;
  class VideoBrokerImpl;
};

namespace Video
{
  // forward declarations
  class Filter;
  class Device;
  class Consumer;
  class FilterTreeParameters;
  class Parameters;

  class Service 
  {
    //    This class starts up the ServiceImpl registers it at the
    //    naming service and runs the orb
  public:
    // Constructor
    Service(Miro::Server& _server,
	    Miro::ConfigDocument * _config,
	    Video::Parameters * _videoParams);
    // Destructor.
    virtual ~Service();

    
  protected:
    Filter * buildFilterTree(Video::Filter * _pre,
			     Miro::ImageFormatIDL const& _format,
			     Video::FilterTreeParameters const& _tree);


  private:
    Video::Device * pVideoDevice_;
    Video::Consumer * pConsumer_;
    Miro::VideoBrokerImpl * pBroker_;
    Miro::VideoBroker_ptr broker_;
    Video::Parameters * videoParameters_;
  };
}
#endif // VideoServer_h
