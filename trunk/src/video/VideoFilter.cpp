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

namespace Video
{
  FILTER_PARAMETERS_FACTORY_IMPL(Filter);

  Filter::Filter(const Miro::ImageFormatIDL& _inputFormat) :
    inputFormat_(_inputFormat),
    outputFormat_(_inputFormat),
    inplace_(false),
    buffer_(new unsigned char[getImageSize(outputFormat_)]),
    interface_(NULL)
  {}

  Filter::~Filter()
  {
    std::cout << "deleting filter " << this->name() << endl;
    std::cout << "deleting successor filters" << endl;
    FilterVector::const_iterator first, last = succ_.end();
    for (first = succ_.begin(); first != last; ++first) {
      delete (*first);
    }

    if (interface_ == NULL) {
      std::cout << "deleting buffer" << endl;
      delete buffer_;
    }
    else {
      std::cout << "deleting interface" << endl;
      delete interface_;
    }
  }

  bool 
  Filter::active() const {
    if (interface_ != NULL && interface_->connections() > 0)
      return true;

    // are any succerssors active?
    FilterVector::const_iterator first, last = succ_.end();
    for (first = succ_.begin(); first != last; ++first) {
      if ((*first)->active()) {
	return true;
      }
    }
    return false;
  }

  void
  Filter::finiTree()
  {
    // finish successors
    FilterVector::const_iterator first, last = succ_.end();
    for (first = succ_.begin(); first != last; ++first) {
      (*first)->finiTree();
    }
    // finish myself
    fini();
  };

  void
  Filter::setInterface(Miro::Server& _server, VideoInterfaceParameters const & _params)
  {
    if (interface_ == NULL)
      delete buffer_;
    else
      delete interface_;
    interface_ = new Miro::VideoImpl(_server, _params, outputFormat_);
    inplace_ = false;
  }

  void
  Filter::init(FilterParameters const *)
  {

  }

  void
  Filter::fini()
  {
  }

  unsigned char const *
  Filter::inputBuffer() {
    assert(pre_ != NULL);
    return pre_->outputBuffer();
  }

  unsigned char *
  Filter::outputBuffer() {
    return buffer_;
  }

  void
  Filter::setBuffer(unsigned char * _buffer)
  {
    buffer_ = _buffer;
  }

  void
  Filter::setPredecessor(Filter * _pre)
  {
    pre_ = _pre;
  }

  void
  Filter::addSuccessor(Filter * _succ)
  {
    succ_.push_back(_succ);
  }

  void
  Filter::acquireOutputBuffer()
  {
    timeStamp_ = (pre_)? pre_->timeStamp() : ACE_OS::gettimeofday();

    if (interface_) {
      bufferIndex_ = interface_->bufferManager()->acquireCurrentReadBuffer();
      buffer_ = interface_->bufferManager()->bufferAddr(bufferIndex_);
    }    
  }

  void
  Filter::releaseOutputBuffer()
  {
    if (interface_) {
      interface_->bufferManager()->bufferTimeStamp(bufferIndex_, timeStamp_);
      interface_->bufferManager()->releaseReadBuffer(bufferIndex_);
    }
  }

  void
  Filter::acquireWriteBuffer()
  {
    timeStamp_ = (pre_)? pre_->timeStamp() : ACE_OS::gettimeofday();

    if (interface_) {
      bufferIndex_ = interface_->bufferManager()->acquireNextWriteBuffer();
      buffer_ = interface_->bufferManager()->bufferAddr(bufferIndex_);
    }    
  }

  void
  Filter::releaseWriteBuffer()
  {
    if (interface_) {
      interface_->bufferManager()->bufferTimeStamp(bufferIndex_, timeStamp_);
      interface_->bufferManager()->releaseWriteBuffer(bufferIndex_);
    }
  }

  void
  Filter::process()
  {
  }


  void
  Filter::processFilterTree() 
  {
    // acquire read buffer
    acquireOutputBuffer();

    // process successors
    FilterVector::const_iterator first, last = succ_.end();
    for (first = succ_.begin(); first != last; ++first) {
      if ((*first)->active()) {
	(*first)->acquireWriteBuffer();
	(*first)->process();
	(*first)->releaseWriteBuffer();
      }
    }

    // release read buffer
    releaseOutputBuffer();

    // process succerssors trees
    for (first = succ_.begin(); first != last; ++first)
      if ((*first)->active()) {
	(*first)->processFilterTree();
      }
  }
};
