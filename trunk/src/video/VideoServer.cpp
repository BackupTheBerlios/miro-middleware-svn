// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
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
#include "idl/ExceptionC.h"
#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/Log.h"

namespace Video
{
  Service::Service(Miro::Server& _server, 
		   Miro::ConfigDocument * _config) :
    schedparams_(ACE_SCHED_FIFO, 10),
    pVideoDevice_(NULL),
    pConsumer_(NULL),
    pBroker_(NULL)
  {
    MIRO_LOG_CTOR("Video::Service");

    Parameters * videoParameters = Parameters::instance();

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
    MIRO_LOG_DTOR("Video::Service");

    pConsumer_->cancel();

    MIRO_DBG(VIDEO, LL_DEBUG, "Video::Device thread canceled.");

    pVideoDevice_->finiTree();
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::FilterTree finished.");

    delete pConsumer_;
    MIRO_DBG(VIDEO, LL_DEBUG, "Video: deleted consumer thread.");

    delete pVideoDevice_;
    MIRO_DBG(VIDEO, LL_DEBUG, "Video: deleted filter tree.");
  }

  Video::Filter *
  Service::buildFilterTree(Video::Filter * _pre,
			   Miro::ImageFormatIDL const& _format,
			   Video::FilterTreeParameters const& _tree) 
  {
    FilterRepository * repo = Video::FilterRepository::instance();
    Filter * filter = repo->getInstance( _tree.type, _format);

    filter->name(_tree.name);
    // set predecessor if available
    if (_pre) {
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
}
