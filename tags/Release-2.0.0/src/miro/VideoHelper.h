// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef MiroVideoHelper_h
#define MiroVideoHelper_h

#include "idl/VideoC.h"
#include "TimeHelper.h"

namespace Miro
{
  //! Get pixel size in byte.
  /** Fails for palettes with none byte aligned pixels like YUV411. */
  unsigned int getPixelSize(VideoPaletteIDL _pal);
  //! Get image size in byte.
  unsigned int getImageSize(ImageFormatIDL const& _format);
  //! Get row size in byte.
  unsigned int getRowSize(ImageFormatIDL const& _format);

  //! Helper class for video connection management.
  /**
   * This class connects in the constructor to the Video object
   * specified as argument. In the destructor it disconnects from the
   * Video object. This class helps avoiding resource leaks, as the
   * connection is automatically reset, as the variable goes out of
   * scope.
   *
   * Connection management is used within the VideoService mainly to
   * skip calculation of filter subtrees no client is connected
   * to. Skipping disconnection as a client therefore can lead to
   * unnecessary filter processing and thus system load.
   */
  class VideoConnection
  {
  public:
    //! Initializing constructor.
    /** @param video Pointer to the Video object, to connect to. */
    VideoConnection(Video_ptr _video);
    //! Resource releasing destructor.
    /** Disconnects from the video object. */
    ~VideoConnection();

    unsigned char const * bufferAddr(CORBA::ULong _index) const;
    
  public:
    //! Pointer to the Video object connected to.
    Miro::Video_var video;
    //! Pointer to the connection handle of type ImageHandleIDL.
    ImageHandleIDL_var handle;
    //! Id of the connection.
    /** Parameter for subsequent calls. */
    CORBA::ULong id;
    //! Pointer to the shared memory buffer of the interface.
    /** 
     * This is NULL if the client is not colocated with the
     * VideoService on the same machine.
     */
    unsigned char const * memory;

  private:
    //! Private copy constructor to avoid accidental usage.
    VideoConnection(VideoConnection const &) {}
  };

  inline
  unsigned char const *
  VideoConnection::bufferAddr(CORBA::ULong _index) const {
    return memory + handle->offset[_index];
  }

  //! Helper class for video image acquisition management.
  /**
   * The Video interface methods acquireCurrentImage and
   * acquireNextImage return the index of the image buffer within the
   * shared memory area of the Video object. It is locked for further
   * writes by the filter. After processing of the image, has to be
   * released to avoid resource leakage within the VideoService. This
   * class helps avoiding resource leaks, as the acquired image is
   * automatically released, as the variable goes out of scope.
   *
   * Note that the filter will not be able to process any new images,
   * if all its buffers are hold by clients. This will happen when a
   * client repeatedly acquires images, and never releases.
   */
  class VideoAcquireImage
  {
  public:
    //! Used to indicate whether the current or the next image is to be acquired.
    enum Selection {Current, Next};

    //! Initializing constructor.
    /**
     * @param connection Reference to the helper class instance.
     * holding the connection to the Video object. 
     * @param select Acquire the current or the next image. Default is next.
     */
    VideoAcquireImage(VideoConnection& _connection,
		      Selection _select = Next);
    //! Resource releasing destructor.
    /** Releases the acquired image. */
    ~VideoAcquireImage();

    //! Pointer to the acquired image.
    unsigned char const * buffer;
    //! Time stamp of the image.
    ACE_Time_Value time;

  protected:
    //! Reference to the helper class instance.
    /** Needed for cleanup. */
    VideoConnection& connection_;
    //! Index of the image buffer within the shared memory area.
    /** Needed for cleanup. */
    CORBA::ULong index_;

  private:
    //! Private copy constructor to avoid accidental usage.
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
    this->buffer = connection_.bufferAddr(index_);
  }

  inline
  VideoAcquireImage::~VideoAcquireImage()
  {
    connection_.video->releaseImage(connection_.id, index_);
  }
};

#endif /* MiroVideoHelper_h */


