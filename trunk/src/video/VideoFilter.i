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
namespace Video
{
  inline
  bool
  Filter::active() const {
    return connected_;
  }

  inline
  unsigned int
  Filter::connections() const {
    return connections_;
  }

  inline
  const Miro::ImageFormatIDL& 
  Filter::inputFormat() const {
    return inputFormat_;
  }
    
  inline
  const Miro::ImageFormatIDL& 
  Filter::outputFormat() const {
    return outputFormat_;
  }
  
  inline
  void
  Filter::name(std::string const& _name) {
    name_ = _name;
  }

  inline
  std::string const&
  Filter::name() const {
    return name_;
  }

  inline
  unsigned char const *
  Filter::inputBuffer() const {
    MIRO_ASSERT(pre_ != NULL);
    return inputBuffer_;
  }

  inline
  unsigned long
  Filter::inputBufferIndex() const {
    return inputBufferIndex_;
  }

  inline
  FilterImageParameters const *
  Filter::inputBufferParameters() const {
    MIRO_ASSERT(pre_ != NULL);
    return inputParameters_;
  }

  inline
  unsigned char *
  Filter::outputBuffer() {
    return outputBuffer_;
  }

  inline
  FilterImageParameters *
  Filter::outputBufferParameters() {
    return outputParameters_;
  }

  inline
  void 
  Filter::inputBuffer(unsigned long _index, 
		      unsigned char const * _buffer,
		      FilterImageParameters const * _params) {
    inputBufferIndex_ = _index;
    inputBuffer_ = _buffer;
    inputParameters_ = _params;
  }

  inline
  unsigned char const * 
  Filter::inputBufferLink(unsigned long _index) const {
    return preLink_[_index].buffer();
  }

  inline
  FilterImageParameters const * 
  Filter::inputBufferParametersLink(unsigned long _index) const {
    return preLink_[_index].params();
  }

  inline
  void 
  Filter::inputBufferLink(unsigned long _linkIndex,
			  unsigned long _bufferIndex, 
			  unsigned char const * _buffer,
			  FilterImageParameters const * _params) {
    preLink_[_linkIndex].buffer(_bufferIndex, _buffer, _params);
  }

  inline
  unsigned int
  Filter::outputBuffers() const throw() {
    MIRO_ASSERT(params_ != NULL);
    return params_->buffers;
  }

  inline
  Miro::VideoImpl *
  Filter::interface() const throw() {
    return interface_;
  }

  inline
  BufferManager *
  Filter::bufferManager() throw() {
    return bufferManager_;
  }

  inline
  BufferManager const *
  Filter::bufferManager() const throw() {
    return bufferManager_;
  }

  inline
  bool
  Filter::interfaceAllowed() const throw()
  {
    return interfaceAllowed_;
  }

  inline
  FilterParameters const *
  Filter::parameters() const throw() {
    return params_;
  }

  inline
  Filter *
  Filter::parentNode() throw() {
    return pre_;
  }

  inline
  unsigned int
  Filter::width() const throw() {
    return inputFormat_.width;
  }

  inline
  unsigned int
  Filter::height() const throw() {
    return inputFormat_.height;
  }
  inline
  ACE_Time_Value const&
  Filter::imageTimeStamp() const {
    return bufferManager()->bufferTimeStamp(outputBufferIndex_);
  }

  inline
  unsigned int
  Filter::number() const throw() {
    return number_;
  }
}
