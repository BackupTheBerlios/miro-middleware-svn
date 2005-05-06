// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "VideoFilter.h"
#include "VideoImpl.h"
#include "VideoDevice.h"

#include "BufferManager.h"

#include "idl/VideoC.h"
#include "miro/VideoHelper.h"
#include "miro/ConfigDocument.h"
#include "miro/Log.h"

namespace Video
{
  Miro::Mutex Filter::connectionMutex_;
  Miro::Condition Filter::connectionCondition_(connectionMutex_);

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
    MIRO_LOG_DTOR("Video::Filter");
    MIRO_DBG(VIDEO, LL_DEBUG, "deleting successor filters");
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
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::Filter::init()");

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

    MIRO_DBG(VIDEO, LL_DEBUG, "Video::Filter::init() end");
  }

  void
  Filter::initTree(Miro::Server& _server, Miro::ConfigDocument& _config) 
  {
    MIRO_DBG(VIDEO, LL_DEBUG, "Video::Filter::initTree()");

    // create an instance of the the filters parameters
    FilterParameters * params = this->getParametersInstance();

    // initialize the parameter instance from the config file
    // _config.getInstance(this->name(), *params);
    _config.getInstance(this->name(), *params);

    // debug output
    MIRO_LOG_OSTR(LL_NOTICE, name() << std::endl << *params);

    // init filter number
    number_ = rootDevice()->getNextFilterNumber();

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
    MIRO_ASSERT(_pre != NULL);

    pre_ = _pre;
    pre_->addSuccessor(this);

    // now we know who our chief is...
    asynchLinkManager_.
      deviceAsynchLinkManager(rootDevice()->deviceAsynchLinkManager());
  }

  /** 
   * Only to be used by Video::Service::buildFilterTree() on filter 
   * construction.
   */
  void
  Filter::addPredecessorLink(Filter * _pre)
  {
    MIRO_ASSERT(_pre != NULL);

    // double link
    preLink_.push_back(FilterPreLink(_pre));
    unsigned int index = preLink_.size() - 1;

    if (_pre->rootNode() == rootNode()) {
      _pre->addSuccessorLink(this, preLink_.size() - 1);
    }
    else {
      AsynchSuccLink succ = asynchLinkManager_.addBufferLink(_pre, index);
      _pre->addAsynchSuccessorLink(succ);
    }
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
  Filter::addAsynchSuccessorLink(AsynchSuccLink const& _succLink)
  {
    asynchSuccLink_.push_back(_succLink);
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
    // std::cout << "try to aquire asynchronous input buffers" << std::endl;

    asynchLinkManager_.getAsynchBuffers(preLink_);

    // std::cout << "aquire output buffer" << std::endl;
    outputBufferIndex_ = bufferManager_->acquireNextWriteBuffer();
    outputBuffer_ = bufferManager_->bufferAddr(outputBufferIndex_);
    outputParameters_ = bufferManager_->imageParameters(outputBufferIndex_);
    if (pre_)
      bufferManager_->
	bufferTimeStamp(outputBufferIndex_,
			pre_->bufferManager_->bufferTimeStamp(inputBufferIndex_));
    
    // FIXME: this does not disable subsequent buffers, if asynch acquire failed.
    
    // std::cout << "process buffer" << std::endl;
    timeFilter_.start();
    process();
    timeFilter_.stop();
    
    
    // std::cout << "relable write buffer as readbuffer for all successors" << std::endl;
    bufferManager_->switchWrite2ReadBuffer(outputBufferIndex_, 
					   successors_ + 
					   brokerLink_.size());
    
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
    
    // std::cout << "set input buffer for all asnychronous successor links." << std::endl;
    {
      AsynchSuccVector::iterator first, last = asynchSuccLink_.end();
      for (first = asynchSuccLink_.begin(); first != last; ++first) {
	first->buffer(bufferManager_->bufferTimeStamp(outputBufferIndex_),
		      outputBufferIndex_,
		      outputBuffer_,
		      outputParameters_);
      }
    }
    
    // std::cout << "set index of all pending broker requests" << std::endl;
    setBrokerRequests();
  
    
    // std::cout << "release read buffer of predecessor" << std::endl;
    if (pre_)
      pre_->bufferManager_->releaseReadBuffer(inputBufferIndex_);

    // std::cout << "release read buffer of all predecessor links" << std::endl;
    {
      FilterPreVector::iterator first, last = preLink_.end();
      for (first = preLink_.begin(); first != last; ++first) {
	if (first->buffer()) {
	  first->filter()->bufferManager_->releaseReadBuffer(first->index());
	  first->buffer(0, NULL, NULL);
	}
      }
    }

    // std::cout << "process asynch filter tree" << std::endl;
    timeFilterTree_.start();
    FilterVector::const_iterator first, last = succ_.end();
    for (first = succ_.begin(); first != last; ++first) {
      if ((*first)->active()) {
	(*first)->processFilterTree();
      }
    }
    timeFilterTree_.stop();
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

  void
  Filter::protectedConnect() 
  {
    // remember to recalculate connectivity.
    rootDevice()->connectionChange();

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

    // process asynchronous predecessor link connections
    asynchLinkManager_.protectedConnect();
  }

  void
  Filter::protectedDisconnect() 
  {
    // process asynchronous predecessor link connections
    asynchLinkManager_.protectedDisconnect();

    // process predecessor link connections
    FilterPreVector::reverse_iterator first, last = preLink_.rend();
    for (first = preLink_.rbegin(); first != last; ++first) {
      first->filter()->protectedDisconnect();
    }

    // process predecessor connection
    if (pre_)
      pre_->protectedDisconnect();
    
    // process filter connection
    --connections_;

    // remember to recalculate connectivity.
    rootDevice()->connectionChange();
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

    //--------------------------------------------------------------------------
    // Calculate the number of successors.
    successors_ = 0;

    // Test all direct successors.
    {
      for (first = succ_.begin(); first != last; ++first) {
	if ((*first)->connected_) {
	  ++successors_;
	}
      }
    }

    {
      // Test all successor links.
      FilterSuccVector::const_iterator first, last = succLink_.end();
      for (first = succLink_.begin(); first != last; ++first) {
	// As connected_ is not yet be initialized on a 
	// forward link we have to check connections_ and
	// not connected_
	// This is only valid as we hold the lock.
	if (first->filter()->connections_ > 0) {
	  ++successors_;
	}
      }
    }

    {
      // Test all asynchronous successor links.
      AsynchSuccVector::iterator first, last = asynchSuccLink_.end();
      for (first = asynchSuccLink_.begin(); first != last; ++first) {
	// As connected_ is not yet be initialized on a 
	// forward link we have to check connections_ and
	// not connected_
	// This is only valid as we hold the lock.
	if (first->protectedCalcConnectivity()) {
	  ++successors_;
	}
      }
    }

    // Set the asynch links of the device, that are
    if (!asynchLinkManager_.empty()) {
      if (connected_)
	asynchLinkManager_.setDeviceAsynchLinksConnected();
      asynchLinkManager_.synchronouslyConnected(connected_);
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

  Device * 
  Filter::rootDevice() throw() 
  {
    return dynamic_cast<Device *>(rootNode());
  }

  void
  Filter::setSynchMode(bool _synchWithCurrent, bool _synchAllNew) throw() 
  {
    // initialize successors
    FilterVector::const_iterator first, last = succ_.end();
    for (first = succ_.begin(); first != last; ++first) {
      (*first)->setSynchMode(_synchWithCurrent, _synchAllNew);
    }

    asynchLinkManager_.setSynchMode(_synchWithCurrent, _synchAllNew);
  }
}
