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
#ifndef Miro_VideoBrokerImpl_h
#define Miro_VideoBrokerImpl_h

#include "idl/VideoS.h"

namespace Video
{
  class Device;
}

namespace Miro
{
  //! Implementation of the Miro::VidoBroker interface.
  class  VideoBrokerImpl : public virtual POA_Miro::VideoBroker
  {
  public:
    //! Initializing constructor.
    /** @param filter The root of the filter tree. */
    VideoBrokerImpl(::Video::Device * _device);
    //! Virtual destructor. 
    virtual ~VideoBrokerImpl();
  
    //! See VideoBroker interface.
    virtual TimeIDL acquireNextImageSet (const ConnectionSetIDL & connections,
					 BufferSetIDL_out buffers)
      ACE_THROW_SPEC ((EOutOfBounds, EDevIO, ETimeOut));

    //! See VideoBroker interface.
    virtual void releaseImageSet (const ConnectionSetIDL & connections,
				  const BufferSetIDL & buffers)
      ACE_THROW_SPEC ((EOutOfBounds));

    //! See VideoBroker interface.
    virtual FilterTreeIDL * filterTreeStats() ACE_THROW_SPEC (());

  protected:
    ::Video::Device * device_;
  };
}

#endif /* Miro_VideoBrokerImpl_h  */
