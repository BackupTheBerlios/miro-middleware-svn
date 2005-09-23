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

#include "VideoDevice.h"

#include "miro/Exception.h"


namespace
{
  struct PalettePair
  {
    std::string first;
    Miro::VideoPaletteIDL second;
  };

  PalettePair palette[Video::Device::NUM_PALETTE_ENTRIES + 2] = {
    {"grey",   Miro::GREY_8},
    {"gray",   Miro::GREY_8},
    {"grey16", Miro::GREY_16},
    {"gray16", Miro::GREY_16},
    {"rgb",    Miro::RGB_24},
    {"bgr",    Miro::BGR_24},
    {"rgba",   Miro::RGB_32},
    {"abgr",   Miro::BGR_32},
    {"yuv",    Miro::YUV_24},
    {"yuv411", Miro::YUV_411},
    {"yuv422", Miro::YUV_422},
    {"yuv420p", Miro::YUV_420P}
  };


};

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(Device);

  //--------------------------------------------------------------------
  Device::Device(const Miro::ImageFormatIDL& _format) :
    Filter(_format),
    mutex_(),
    condition_(mutex_),
    connectionChange_(0)
  {
    // Forbid instances of the video interface here.
    interfaceAllowed_ = false;

    // clear table of supported palettes
    for (unsigned int i = 0; i < NUM_PALETTE_ENTRIES; ++i)
      paletteLookup[i] = -1;
  }

  //--------------------------------------------------------------------
  Device::~Device()
  {
  }

//   void
//   Device::init(Miro::Server & _server, Video::FilterParameters const * _params) 
//   {
//     Super::init(_server, _params);
//   }

  //--------------------------------------------------------------------
  void
  Device::skipImage() 
  {
    outputBufferIndex_ = bufferManager_->acquireNextWriteBuffer();
    outputBuffer_ = bufferManager_->bufferAddr(outputBufferIndex_);
    outputParameters_ = bufferManager_->imageParameters(outputBufferIndex_);
    
    timeFilter_.start();
    process();
    timeFilter_.done();

    bufferManager_->switchWrite2ReadBuffer(outputBufferIndex_, 0);
  }

  //--------------------------------------------------------------------
  void
  Device::enqueueBrokerRequests(BrokerRequestVector const& _request)
  {
    MIRO_DBG(VIDEO, LL_PRATTLE, "Device::enqueueBrokerRequests()");

    Miro::Guard guard(mutex_);
    brokerRequest_.reserve(brokerRequest_.size() + _request.size());
    BrokerRequestVector::const_iterator first, last = _request.end();
    for (first = _request.begin(); first != last; ++first) {
      brokerRequest_.push_back(*first);
    }
  }

  //--------------------------------------------------------------------
  void
  Device::setBrokerRequestQueue()
  {
    MIRO_DBG(VIDEO, LL_PRATTLE, "Device::setBrokerRequestQueue()");

    Miro::Guard guard(mutex_);
    BrokerRequestVector::const_iterator first, last = brokerRequest_.end();
    for (first = brokerRequest_.begin(); first != last; ++first) {
      first->filter->addBrokerRequest(first->link);
    }
    brokerRequest_.clear();
  }

  //--------------------------------------------------------------------
  Miro::VideoPaletteIDL
  Device::getPalette(const std::string & pal)
  {
    for (unsigned int i = 0; i < NUM_PALETTE_ENTRIES + 2; ++i) {
      if (pal == palette[i].first) {
	return palette[i].second;
      }
    }

    throw Miro::Exception(std::string("Video::Parameters: unknown palette: ") + pal);
  }

  /**
   * Due to synchronization and locking issues this has to be
   * calculated before processing a filter tree.
   *
   * Returns true if the connectivity of connected asynchronous trees was
   * also updated.
   */
  bool
  Device::calcConnectivity()
  {
    if (connectionChange_ == 0)
      return true;

    Miro::Guard guard(connectionMutex_);

    //    cout << name() << " calcConnectivity\n";
    if (connectionChange_ == 2) {
      // reset connectivity status of the asynchronous links.
      deviceAsynchLinkManager_.clearConnectivityStatus();

      // calculate the connectivity status of the synchronous links      
      protectedCalcConnectivity();
      connectivityTimeStamp_ = ACE_OS::gettimeofday();
      connectionChange_ = 1;
      connectionCondition_.broadcast();
    }

    if (deviceAsynchLinkManager_.asynchBuffersConnected(connectionChangeTimeStamp_)) {
      connectionChange_ = 0;
      return true;
    }

    // wait for the next update, to prevent busy waiting
    ACE_Time_Value maxWait = ACE_OS::gettimeofday() + ACE_Time_Value(0, 500000);
    if (connectionCondition_.wait(&maxWait) == -1) {
      MIRO_LOG_OSTR(LL_WARNING, "VideoDevice: " << name() << 
		    "Timeout on wait asynch connectivity update.");
    }
    return false;
  }

  void
  Device::synchModel(bool _synchWithCurrent, bool _synchAllNew) throw() {
    deviceAsynchLinkManager_.synchMode(_synchWithCurrent, _synchAllNew);
    setSynchMode(_synchWithCurrent, _synchAllNew);
  }
  
}

