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
#include "VideoBrokerImpl.h"
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
    pConsumer_(NULL),
    pBroker_(NULL)
  {
    DBG(std::cout << "VideoService initializing.." << endl);

    Parameters * videoParameters = Parameters::instance();

    std::cout << "Setting up filter tree." << endl;
    
    _config->setSection("Video");
    Miro::ImageFormatIDL format;
    format.width = videoParameters->width;
    format.height = videoParameters->height;
    format.palette = Video::Device::getPalette(videoParameters->palette);
    Filter * filter = buildFilterTree(NULL,
				      format,
				      videoParameters->filter);

    pVideoDevice_ = dynamic_cast<Video::Device *>(filter);
    assert (pVideoDevice_ != NULL);

    pVideoDevice_->initTree(_server, *_config);

    pConsumer_ = new Video::Consumer(*pVideoDevice_, &schedparams_);
    pConsumer_->open(NULL);

    pBroker_ = new Miro::VideoBrokerImpl(pVideoDevice_);
    broker_ = pBroker_->_this();
    _server.addToNameService(broker_, "VideoBroker");
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
  Service::buildFilterTree(Video::Filter * _pre,
			   Miro::ImageFormatIDL const& _format,
			   Video::FilterTreeParameters const& _tree) 
  {
    cout << "get the filter repository instance" << endl;
    FilterRepository * repo = Video::FilterRepository::instance();
    cout << " create a new filter instance. " << endl;
    Filter * filter = repo->getInstance( _tree.type, _format);

    cout << "set the filter name " << _tree.name << endl;
    filter->name(_tree.name);
    // set predecessor if available
    if (_pre) {
      cout << "set predecessor." << endl;
      filter->setPredecessor(_pre);
    }

    // set predecessor links
    std::vector<std::string>::const_iterator first, last = _tree.backLink.end();
    for (first = _tree.backLink.begin(); first != last; ++first) 
      filter->addPredecessorLink(filter->findByName(*first));

    // build successor filters
    for (unsigned int i = 0; i < _tree.successor.size(); ++i)
      buildFilterTree(filter,
		      filter->outputFormat(),
		      _tree.successor[i]);
  
    return filter;
  }


};
