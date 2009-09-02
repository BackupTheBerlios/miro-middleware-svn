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
#ifndef OdometryTracking_h
#define OdometryTracking_h

#include "idl/MotionStatusC.h"
#include "StructuredPushConsumer.h"

#include <list>
#include <string>

namespace Miro
{
  //! Odometry event caching and position interpolation.
  /**
   * Class for local odometry event caching.
   * OdometryTracking builds a history of passed odometry events, that
   * can be used for timestamp based position retreaval and sub event
   * resolution position interpolation.
   */
  class OdometryTracking : public StructuredPushConsumer
  {
    //! Super class type.
    typedef StructuredPushConsumer Super;

  public:
    //! Initializing constructor.
    OdometryTracking(CosNotifyChannelAdmin::EventChannel_ptr _ec,
                     const std::string& domainName);

    //! CORBA StructuredPushConsumer interface method implementation.
    virtual void push_structured_event(const CosNotification::StructuredEvent & notification
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    throw(CORBA::SystemException, CosEventComm::Disconnected);

    //! Query position for a discrete time.
    MotionStatusIDL getPosition(const ACE_Time_Value& stamp) const;
    //! Query raw position for a discrete time.
    PositionIDL getRawPosition(const ACE_Time_Value& stamp) const;

  protected:
    //! History size.
    static const unsigned long maxDequeSize = 128;

    //! Odometry event history type.
    /**
     * Use of std::deque is hindered by a bug in the implementation
     * for gcc 2.95.2. (To be revisited when downward compability
     * gets dropped.)
     */
    typedef std::list<Miro::MotionStatusIDL> OdometryDeque;
    //! RawPosition event history size.
    /**
     * Use of std::deque is hindered by a bug in the implementation
     * for gcc 2.95.2. (To be revisited when downward compability gets
     * dropped.)
     */
    typedef std::list<Miro::RawPositionIDL> RawDeque;

    //! Mutex for Odometry event tracking.
    mutable Miro::Mutex odoMutex;
    //! Mutex for RawPosition event tracking.
    mutable Miro::Mutex rawMutex;
    //! Odometry event history.
    OdometryDeque odoDeque;
    //! If set true, max Odometry event history size is reached.
    bool odoTruncate_;
    //! RawPosition event history.
    RawDeque rawDeque;
    //! If set true, max RawPosition event history size is reached.
    bool rawTruncate_;
  };
};

#endif
