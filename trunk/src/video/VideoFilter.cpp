// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "VideoFilter.h"
#include "VideoImpl.h"

#include "BufferManager.h"

#include "miro/VideoHelper.h"
#include "idl/VideoC.h"
#include "miro/ConfigDocument.h"

namespace Video
{
  Miro::Mutex Filter::connectionMutex_;

  FILTER_PARAMETERS_FACTORY_IMPL(Filter);
  IMAGE_PARAMETERS_FACTORY_IMPL(Filter);

  /**
   * The constructor of the derived filter is responsible for
   * checking, whether the requested input format is valid. Also, it
   * is responsible, that the member struct outputFormat_ is
   * initialized correctly. The base class just copies the value of
   * the provided input format into the member outputFormat_.
   *
   * @param inputFormat The requested input format for the filter,
   * as defined by the @ref outputFormat() of the predecessor
   * filter. For root filters, it is defined by the configuration
   * file.
   *
   * @throw Miro::Exception is thrown to indicate, that the
   * inputFormat is not supported by the filter.
   */
  Filter::Filter(const Miro::ImageFormatIDL& _inputFormat) :
    inputFormat_(_inputFormat),
    outputFormat_(_inputFormat),
    interfaceAllowed_(true),
    inplace_(false),
    params_(NULL),
    bufferManager_(NULL),
    inputBufferIndex_(0),
    inputBuffer_(NULL),
    outputBufferIndex_(0),
    outputBuffer_(NULL),
    interface_(NULL),
    pre_(NULL),
    connections_(0),
    connected_(false),
    successors_(0)
  {}

  /**
   * Deletes all successor filters in reverse order of construction.
   */
  Filter::~Filter()
  {
    std::cout << "deleting filter " << this->name() << std::endl;
    std::cout << "deleting successor filters" << std::endl;
    FilterVector::const_reverse_iterator first, last = succ_.rend();
    for (first = succ_.rbegin(); first != last; ++first) {
      delete (*first);
    }
    delete interface_;
    delete bufferManager_;
  }


  BufferManager * 
  Filter::bufferManagerInstance() const 
  {
    return new BufferManager(this, 
			     params_->buffers, 
			     getImageSize(outputFormat()));
  }

  /**
   * After construction of the filter tree, all filters are
   * initialized in depth first order of the filter tree description
   * of the configuration file.
   *
   * @param server The reference to a Miro::Server object for
   * accessing the ORB, POA and NamingService.
   *
   * @param params The filter parameters as specified in the
   * configuration file. The type is that returned by the filter
   * parameter factory method of the class.
   */
  void
  Filter::init(Miro::Server& _server, FilterParameters const * _params)
  {
    std::cout << "Filter::init" << std::endl;

    // safe instance, as we have to clean it up.
    params_ = _params;

    // Test for inplace:
    // FIXME: We can't do inplace up till now.
    if (inplace_) {
	inplace_ = false;
    }

    // create the interface.
    if (_params->interfaceInstance) {
      if (interfaceAllowed()) {
	interface_ = new Miro::VideoImpl(_server, _params->interface, this);
	bufferManager_ = interface_->bufferManager();
      }
      else
	throw Miro::Exception("Device::setInterface not supported.");    
    }
    else {
      bufferManager_ = bufferManagerInstance();
    }

    std::cout << "Filter::init end" << std::endl;
  }

  void
  Filter::initTree(Miro::Server& _server, Miro::ConfigDocument& _config) 
  {
    std::cout << "initTree: " << name() << std::endl;

    // create an instance of the the filters parameters
    FilterParameters * params = this->getParametersInstance();

    // initialize the parameter instance from the config file
    // _config.getInstance(this->name(), *params);
    _config.getParameters(this->name(), *params);

    // debug output
    std::cout << name() << std::endl;
    std::cout << *params << std::endl;

    // initialize the filter instance with its parameters
    this->init(_server, params);

    // initialize successors
    FilterVector::const_iterator first, last = succ_.end();
    for (first = succ_.begin(); first != last; ++first) {
      (*first)->initTree(_server, _config);
    }
  }

  /**
   * Before destruction of the filter tree, all filters are
   * finalized in reversed prefix depth first order of the filter
   * tree description of the configuration file.
   */
  void
  Filter::fini()
  {
    delete interface_;
    if (interface_ == NULL) {
      delete bufferManager_;
    }
    delete params_;
    
    interface_ = NULL;
    bufferManager_ = NULL;
    params_ = NULL;
  }

  /**
   * It uses the reversed prefix depth first traversion of the tree,
   * taking this filter as root.
   */
  void
  Filter::finiTree()
  {
    // finalize successors
    FilterVector::const_reverse_iterator first, last = succ_.rend();
    for (first = succ_.rbegin(); first != last; ++first) {
      (*first)->finiTree();
    }
    // finalize myself
    fini();
  };

  /** 
   * Only to be used by Video::Service::buildFilterTree() on filter 
   * construction.
   */
  void
  Filter::setPredecessor(Filter * _pre)
  {
    assert(_pre != NULL);

    pre_ = _pre;
    pre_->addSuccessor(this);
  }

  /** 
   * Only to be used by Video::Service::buildFilterTree() on filter 
   * construction.
   */
  void
  Filter::addPredecessorLink(Filter * _pre)
  {
    assert(_pre != NULL);

    // double link
    preLink_.push_back(FilterPreLink(_pre));
    _pre->addSuccessorLink(this, preLink_.size() - 1);
  }

  void
  Filter::addSuccessor(Filter * _succ)
  {
    succ_.push_back(_succ);
  }

  void
  Filter::addSuccessorLink(Filter * _succ, unsigned long _index)
  {
    succLink_.push_back(FilterSuccLink(_succ, _index));
  }

  void
  Filter::process()
  {
  }

  /**
   * It uses the prefix depth first traversion of the tree,
   * taking this filter as root.
   */
  void
  Filter::processFilterTree() 
  {
    // std::cout << "aquire output buffer" << std::endl;
    outputBufferIndex_ = bufferManager_->acquireNextWriteBuffer();
    outputBuffer_ = bufferManager_->bufferAddr(outputBufferIndex_);
    outputParameters_ = bufferManager_->imageParameters(outputBufferIndex_);
    if (pre_)
      bufferManager_->
	bufferTimeStamp(outputBufferIndex_,
			pre_->bufferManager_->bufferTimeStamp(inputBufferIndex_));

    // std::cout << "process buffer" << std::endl;
    timeFilter_.start();
    process();
    timeFilter_.stop();

    // std::cout << "relable write buffer as readbuffer for all successors" << std::endl;
    bufferManager_->switchWrite2ReadBuffer(outputBufferIndex_, 
					   successors_ + brokerLink_.size());

    // std::cout << "set input buffer for all successors." << std::endl;
    {
      FilterVector::const_iterator first, last = succ_.end();
      for (first = succ_.begin(); first != last; ++first) {
	if ((*first)->active()) {
	  (*first)->inputBuffer(outputBufferIndex_, 
				outputBuffer_,
				outputParameters_);
	}
      }
    }

    // std::cout << "set input buffer for all successor links." << std::endl;
    {
      FilterSuccVector::iterator first, last = succLink_.end();
      for (first = succLink_.begin(); first != last; ++first) {
	if (first->filter()->active()) {
	  first->filter()->inputBufferLink(first->index(), 
					   outputBufferIndex_,
					   outputBuffer_,
					   outputParameters_);
	}
      }
    }

    // std::cout << "set index of all pending broker requests" << std::endl;
    setBrokerRequests();

    // std::cout << "release read buffer of predecessor" << std::endl;
    if (pre_)
      pre_->bufferManager_->releaseReadBuffer(inputBufferIndex_);

    // std::cout << "release read buffer of all predecessor links" << std::endl;
    {
      FilterPreVector::const_iterator first, last = preLink_.end();
      for (first = preLink_.begin(); first != last; ++first) {
	first->filter()->bufferManager_->releaseReadBuffer(first->index());
      }
    }

    // std::cout << "process successors" << std::endl;
    {
      timeFilterTree_.start();
      FilterVector::const_iterator first, last = succ_.end();
      for (first = succ_.begin(); first != last; ++first) {
	if ((*first)->active()) {
	  (*first)->processFilterTree();
	}
      }
      timeFilterTree_.stop();
    }
  }

  /**
   * It uses the prefix depth first traversion of the tree,
   * taking this filter as root.
   *
   * @param tree The filter statistics tree, to store the data.
   */
  void
  Filter::filterTreeStats(Miro::FilterTreeIDL& _tree) const
  {
    _tree.name = CORBA::string_dup(name_.c_str());
    _tree.connections = connections_;
    _tree.videoInterface.ior = Miro::Video::_nil();

    Miro::TimeStats t;

    timeFilter_.eval(t);
    Miro::timeA2C(t.min, _tree.timeFilter.min);
    Miro::timeA2C(t.max, _tree.timeFilter.max);
    Miro::timeA2C(t.mean, _tree.timeFilter.mean);
    Miro::timeA2C(t.var, _tree.timeFilter.var);

    timeFilterTree_.eval(t);
    Miro::timeA2C(t.min, _tree.timeSubtree.min);
    Miro::timeA2C(t.max, _tree.timeSubtree.max);
    Miro::timeA2C(t.mean, _tree.timeSubtree.mean);
    Miro::timeA2C(t.var, _tree.timeSubtree.var);

    if (interface_ != NULL) {
      _tree.videoInterface.ior = Miro::Video::_duplicate(interface_->ior());
      _tree.videoInterface.connections = interface_->connections();
      _tree.videoInterface.name = CORBA::string_dup(interface_->name().c_str());
    }

    _tree.successorLinks.length(succLink_.size());
    for (unsigned int i = 0; i != _tree.successorLinks.length(); ++i) {
      _tree.successorLinks[i] = CORBA::string_dup(succLink_[i].filter()->name().c_str());
    }

    _tree.successors.length(succ_.size());
    for (unsigned int i = 0; i != _tree.successors.length(); ++i) {
      succ_[i]->filterTreeStats(_tree.successors[i]);
    }
  }

  void 
  Filter::connect()
  {
    Miro::Guard guard(connectionMutex_);
    protectedConnect();
  }
  

  void
  Filter::disconnect()
  {
    Miro::Guard guard(connectionMutex_);
    protectedDisconnect();
  }

  /**
   * Due to synchronization and locking issues this has to be
   * calculated before processing a filter tree.
   */
  void
  Filter::calcConnectivity()
  {
    Miro::Guard guard(connectionMutex_);
    protectedCalcConnectivity();
  }


  void
  Filter::protectedConnect() 
  {
    // process filter connection
    ++connections_;

    // process predecessor connection
    if (pre_)
      pre_->protectedConnect();

    // process predecessor link connections
    FilterPreVector::iterator first, last = preLink_.end();
    for (first = preLink_.begin(); first != last; ++first) {
      first->filter()->protectedConnect();
    }
  }

  void
  Filter::protectedDisconnect() 
  {
    // process predecessor link connections
    FilterPreVector::iterator first, last = preLink_.end();
    for (first = preLink_.begin(); first != last; ++first) {
      first->filter()->protectedDisconnect();
    }

    // process predecessor connection
    if (pre_)
      pre_->protectedDisconnect();
    
    // process filter connection
    --connections_;
  }

  void
  Filter::protectedCalcConnectivity()
  {
    // Depth first recursion
    FilterVector::const_iterator first, last = succ_.end();
    for (first = succ_.begin(); first != last; ++first) {
      (*first)->protectedCalcConnectivity();
    }

    // The filter is connected, if it has any connections.
    // This includes successors and successor links.
    connected_ = connections_ > 0;

    // Calculate the number of successors.
    successors_ = 0;
    // Test all direct successors.
    for (first = succ_.begin(); first != last; ++first) {
      if ((*first)->connected_) {
	++successors_;
      }
    }
    // Test all successor links.
    FilterSuccVector::const_iterator f, l = succLink_.end();
    for (f = succLink_.begin(); f != l; ++f) {
      // As connected_ is not yet be initialized on a 
      // forward link we have to check connections_ and
      // not connected_
      // This is only valid as we hold the lock.
      if (f->filter()->connections_ > 0) {
	++successors_;
      }
    }
  }

  /**
   * @param name The filter name to find.
   * @ret A pointer to the filter. NULL if no such filter is found.
   */
  Filter *
  Filter::findByName(std::string const & _name)
  {
    // climb up the filter tree
    if (pre_ != NULL) {
      return pre_->findByName(_name);
    }
    else {
      return findByNameDown(_name);
    }
  }

  /**
   * @param name The filter name to find.
   * @ret A pointer to the filter. NULL if no such filter is found.
   */
  Filter *
  Filter::findByInterfaceName(std::string const & _name)
  {
    Filter * filter = NULL;
    if (interface_ != NULL && 
	interface_->name() == _name)
      filter = this;
    else {
      FilterVector::const_iterator first, last = succ_.end();
      for (first = succ_.begin(); first != last; ++first) {
	filter = (*first)->findByInterfaceName(_name);
	if (filter != NULL) {
	  break;
	}
      }
    }
    return filter;
  }

  Filter * 
  Filter::findByNameDown(std::string const & _name) {
    Filter * filter = NULL;
    if (name_ == _name)
      filter = this;
    else {
      FilterVector::iterator first, last = succ_.end();
      for (first = succ_.begin(); first != last; ++first) {
	filter = (*first)->findByNameDown(_name);
	if (filter != NULL) {
	  break;
	}
      }
    }
    return filter;
  }

  void 
  Filter::setBrokerRequests() 
  {
    // std::cout << __PRETTY_FUNCTION__ << std::endl;

    BrokerLinkVector::const_iterator first, last = brokerLink_.end();
    for (first = brokerLink_.begin(); first != last; ++first) {
      (*first)->setIndex(outputBufferIndex_);
    }
    brokerLink_.clear();
  }

  void
  Filter::addBrokerRequest(BrokerLink * _brokerRequest)
  {
    // std::cout << __PRETTY_FUNCTION__ << std::endl;

    brokerLink_.push_back(_brokerRequest);
  }

  Filter * 
  Filter::rootNode() throw() 
  {
    if (pre_ != NULL) {
      MIRO_ASSERT(pre_ != this);
      return pre_->rootNode();
    }
    return this;
  }
}
