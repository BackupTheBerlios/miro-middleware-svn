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
#ifndef Video_Filter_h
#define Video_Filter_h

#include "Parameters.h"

#include "miro/VideoC.h"

#include <vector>

#define FILTER_PARAMETERS_FACTORY(X) \
    virtual X ## Parameters * getParametersInstance() const; \
    virtual X ## Parameters * getParametersInstance(const ::Video::FilterParameters& _p) const

#define FILTER_PARAMETERS_FACTORY_IMPL(X) \
  X ## Parameters * \
  X::getParametersInstance() const \
  { \
    return new X ## Parameters(); \
  } \
  X ## Parameters * \
  X::getParametersInstance(const ::Video::FilterParameters& _p) const \
  { \
    X ## Parameters const& p= dynamic_cast<X ## Parameters const&>(_p); \
    return new X ## Parameters(p); \
  }

namespace Miro
{
  class Server;
  class VideoImpl;
};

namespace Video
{
  class FilterParameters;
  class VideoInterfaceParameters;

  class Filter
  {
  public:
    Filter(const Miro::ImageFormatIDL& _inputFormat);
    virtual ~Filter();

    FILTER_PARAMETERS_FACTORY(Filter);

    virtual void setBuffer(unsigned char * _buffer);
    virtual void setInterface(Miro::Server& _server, VideoInterfaceParameters const & _params);
    const Miro::ImageFormatIDL& inputFormat() const;
    const Miro::ImageFormatIDL& outputFormat() const;

    bool inplace() const;
    virtual void init(FilterParameters const *);
    virtual void fini();

    virtual void acquireWriteBuffer();
    virtual void releaseWriteBuffer();
    virtual void acquireOutputBuffer();
    virtual void releaseOutputBuffer();

    virtual void process();
    virtual void processFilterTree();

    void setPredecessor(Filter * _pre);
    void addSuccessor(Filter * _succ);
    
    ACE_Time_Value const& timeStamp() const;
  protected:
    typedef std::vector<Filter *> FilterVector;

    unsigned char const * inputBuffer();
    unsigned char * outputBuffer();

    Miro::ImageFormatIDL inputFormat_;
    Miro::ImageFormatIDL outputFormat_;
    bool inplace_;

    ACE_Time_Value timeStamp_;
    unsigned int bufferIndex_;
    unsigned char * buffer_;
    Miro::VideoImpl * interface_;

    Filter * pre_;
    FilterVector succ_;
  };

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
  ACE_Time_Value const&
  Filter::timeStamp() const {
    return timeStamp_;
  }
};

#endif
