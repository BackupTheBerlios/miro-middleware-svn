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
#ifndef MiroVideoHelper_h
#define MiroVideoHelper_h

#include "VideoC.h"
#include "TimeHelper.h"

namespace Miro
{
  unsigned int getPixelSize(VideoPaletteIDL _pal);
  unsigned int getImageSize(ImageFormatIDL const& _format);
  unsigned int getRowSize(ImageFormatIDL const& _format);

  class VideoConnection
  {
  public:
    VideoConnection(Video_ptr _video);
    ~VideoConnection();

  public:
    Miro::Video_ptr video;
    ImageHandleIDL_var handle;
    CORBA::ULong id;

    unsigned char * memory;

  private:
    VideoConnection(VideoConnection const &) {}
  };


  class VideoAcquireImage
  {
  public:
    enum Selection {Current, Next};

    VideoAcquireImage(VideoConnection& _connection,
		      Selection _select);
    ~VideoAcquireImage();

    unsigned char * buffer;
    ACE_Time_Value time;

  protected:
    VideoConnection& connection_;
    CORBA::ULong index_;

  private:
    VideoAcquireImage(VideoAcquireImage const& i)  :
      connection_(i.connection_)
    {}
  };

  inline
  VideoAcquireImage::VideoAcquireImage(VideoConnection& _connection,
				       Selection _select) :
    connection_(_connection)
  {
    TimeIDL t = (_select == Current)?
      connection_.video->acquireCurrentImage(connection_.id, index_) :
      connection_.video->acquireNextImage(connection_.id, index_);
    timeC2A(t, this->time);
    this->buffer = connection_.memory + connection_.handle->offset[index_];
  }

  inline
  VideoAcquireImage::~VideoAcquireImage()
  {
    connection_.video->releaseImage(connection_.id, index_);
  }
};

#endif /* MiroVideoHelper_h */


