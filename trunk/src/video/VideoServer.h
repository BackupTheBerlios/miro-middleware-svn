// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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
	    Video::Parameters * _videoParams = Video::Parameters::instance());
    // Destructor.
    virtual ~Service();

    
  protected:
    Filter * buildFilterTree(Video::Filter * _pre,
			     Miro::ImageFormatIDL const& _format,
			     Video::FilterTreeParameters const& _tree);


  private:
    /** Sceduling parameters for a realtime thread */
    ACE_Sched_Params schedparams_;
    Video::Device * pVideoDevice_;
    Video::Consumer * pConsumer_;
    Miro::VideoBrokerImpl * pBroker_;
    Miro::VideoBroker_ptr broker_;
    Video::Parameters * videoParameters_;
  };
}
#endif // VideoServer_h
