// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "VideoServer.h"
#include "VideoDevice.h"
#include "VideoConsumer.h"
#include "VideoFilterRepository.h"
#include "Parameters.h"

#include "miro/Server.h"
#include "miro/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"

#include <iostream>

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Video
{
  Service::Service(Miro::Server& _server, 
		   Miro::ConfigDocument * _config) :
    schedparams_(ACE_SCHED_FIFO, 10),
    pVideoDevice_(NULL),
    pConsumer_(NULL)
  {
    DBG(std::cout << "VideoService initializing.." << endl);

    Parameters * videoParameters = Parameters::instance();

    std::cout << "Setting up filter tree." << endl;
    
    _config->setSection("Video");
    Miro::ImageFormatIDL format;
    format.width = videoParameters->width;
    format.height = videoParameters->height;
    format.palette = Video::Device::getPalette(videoParameters->palette);
    Filter * filter = buildFilterTree(_server,
				      NULL,
				      format,
				      _config,
				      videoParameters->filter);

    pVideoDevice_ = dynamic_cast<Video::Device *>(filter);
    assert (pVideoDevice_ != NULL);

    pConsumer_ = new Video::Consumer(*pVideoDevice_, &schedparams_);
    pConsumer_->open(NULL);
  }

  Service::~Service()
  {
    DBG(std::cout << "Destructing VideoService." << endl);

    pConsumer_->cancel();

    std::cout << "Video::Device thread canceled." << endl;
    pVideoDevice_->finiTree();

    std::cout << "Video::FilterTree finished." << endl;

    delete pConsumer_;

    std::cout << "Video: deleted consumer thread" << endl;

    delete pVideoDevice_;
    std::cout << "Video: deleted filter tree." << endl;
  }

  Video::Filter *
  Service::buildFilterTree(Miro::Server& _server,
			   Video::Filter * _pre,
			   Miro::ImageFormatIDL const& _format,
			   Miro::ConfigDocument * _config,
			   Video::FilterTreeParameters const& _tree) 
  {
    FilterRepository * repo = Video::FilterRepository::instance();
  
    Filter * filter = repo->getInstance( _tree.type, _format);
    FilterParameters * params = filter->getParametersInstance();

    _config->getParameters(_tree.name, *params);

    std::cout << _tree.name << endl;
    std::cout << *params << endl;

    filter->name(_tree.name);
    filter->init(params);

    if (_pre) {
      filter->setPredecessor(_pre);
      _pre->addSuccessor(filter);
    }
    if (params->interfaceInstance)
      filter->setInterface(_server, params->interface);
  
    for (unsigned int i = 0; i < _tree.successor.size(); ++i)
      buildFilterTree(_server,
		      filter,
		      filter->outputFormat(),
		      _config,
		      _tree.successor[i]);
  
    return filter;
  }

};
