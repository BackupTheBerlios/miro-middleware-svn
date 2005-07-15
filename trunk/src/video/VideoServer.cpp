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

#include <map>

namespace {
  typedef std::map<std::string, Video::Filter *> FilterNameMap;
  FilterNameMap filterNameMap;
}

namespace Video
{

  Service::Service(Miro::Server& _server, 
		   Miro::ConfigDocument * _config,
		   Parameters * _videoParams) :
    pVideoDevice_(NULL),
    pConsumer_(NULL),
    pBroker_(NULL),
    videoParameters_(_videoParams)
  {
    MIRO_LOG_CTOR("Video::Service");

    //Parameters * videoParameters = Parameters::instance();

    _config->setSection("Video");
    Miro::ImageFormatIDL format;
    format.width = videoParameters_->width;
    format.height = videoParameters_->height;
    format.palette = Video::Device::getPalette(videoParameters_->palette);
    Filter * filter = buildFilterTree(NULL,
				      format,
				      videoParameters_->filter);

    pVideoDevice_ = dynamic_cast<Video::Device *>(filter);
    assert (pVideoDevice_ != NULL);

    pVideoDevice_->initTree(_server, *_config);

    // configure synchronization model
    pVideoDevice_->
      synchModel(videoParameters_->synchWithCurrent,
		 videoParameters_->synchAllNew);
    
    pConsumer_ = new Video::Consumer(*pVideoDevice_, &videoParameters_->scheduling);
    pConsumer_->setMaxSynchJitter(videoParameters_->maxSynchJitter);
    pConsumer_->open(NULL);

    pBroker_ = new Miro::VideoBrokerImpl(pVideoDevice_);
    broker_ = pBroker_->_this();
    _server.addToNameService(broker_, videoParameters_->videoBroker);
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
    if (filterNameMap.find(_tree.name) != filterNameMap.end())
      throw Miro::Exception("Filter name not unique: " + _tree.name);

    FilterRepository * repo = Video::FilterRepository::instance();
    Filter * filter = repo->getInstance( _tree.type, _format);

    filter->name(_tree.name);
    // set predecessor if available
    if (_pre) {
      filter->setPredecessor(_pre);
    }

    // set predecessor links
    std::vector<std::string>::const_iterator first, last = _tree.backLink.end();
    for (first = _tree.backLink.begin(); first != last; ++first) {
      Filter * preLink = 0;
      FilterNameMap::const_iterator i = filterNameMap.find(*first);
      if (i != filterNameMap.end())
	preLink = i->second;
      filter->addPredecessorLink(preLink);
    }

    // add filter name to filter name map for referencation
    filterNameMap.insert(std::make_pair(filter->name(), filter));

    // build successor filters
    for (unsigned int i = 0; i < _tree.successor.size(); ++i)
      buildFilterTree(filter,
		      filter->outputFormat(),
		      _tree.successor[i]);
  

    return filter;
  }
}
