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
//
// Authors:
//   Hans Utz
//   Philipp Baer
//
#ifndef NotifyMulticastSender_h
#define NotifyMulticastSender_h

/* TAO includes */
#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

/* ACE includes */
#include <ace/SOCK_Dgram_Mcast.h>

/* Miro includes */
#include <miro/StructuredPushConsumer.h>

#if GCC_MAJOR_VERSION == 2 && GCC_MINOR_VERSION == 96
#include <list>
#else
#include <deque>
#endif

namespace Miro
{
  namespace NMC {
    // forward declerations
    class Config;
    class SH;
    class EventFilter;
    class Parameters;

    class Sender : public Miro::StructuredPushConsumer
    {
      //--------------------------------------------------------------------------
      // private types
      //--------------------------------------------------------------------------
      typedef Miro::StructuredPushConsumer Super;

    public:
      //--------------------------------------------------------------------------
      // public methods
      //--------------------------------------------------------------------------
      Sender(ACE_SOCK_Dgram_Mcast& _socket,
             CosNotifyChannelAdmin::EventChannel_ptr _ec,
             std::string const& _domainName,
             Parameters * _parameters);

      //! Default destructor.
      ~Sender();

      void init();
      int sendData(iovec *_iov, size_t _iovLen);
      void setEventFilter(EventFilter * _event_filter);

      friend class SH;
    protected:
      //--------------------------------------------------------------------------
      // protected types
      //--------------------------------------------------------------------------
      typedef std::pair<CORBA::ULong, CORBA::ULong> TimeSizePair;
#if GCC_MAJOR_VERSION == 2 && GCC_MINOR_VERSION == 96
      typedef std::list<TimeSizePair> BandwidthQueue;
#else
      typedef std::deque<TimeSizePair> BandwidthQueue;
#endif

      //--------------------------------------------------------------------------
      // protected methods
      //--------------------------------------------------------------------------
      void subscribe(std::string const& _domain,
                     std::string const& _type)
      throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

      void unsubscribe(std::string const& _domain,
                       std::string const& _type)
      throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

      void marshalEvent(const CosNotification::StructuredEvent &_event);

      void sendCdrStream(TAO_OutputCDR &cdr);

      unsigned long int getFragmentCount(const ACE_Message_Block *_begin,
                                         const ACE_Message_Block *_end,
                                         int _iovSize,
                                         CORBA::ULong _maxFragmentPayload,
                                         CORBA::ULong &_totalLength);

      void sendFragment(CORBA::ULong _requestId,
                        CORBA::ULong _requestSize,
                        CORBA::ULong _fragmentSize,
                        CORBA::ULong _fragmentOffset,
                        CORBA::ULong _fragmentId,
                        CORBA::ULong _fragmentCount,
                        CORBA::ULong _timestamp,
                        iovec _iov[],
                        int _iovcnt);

      void disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException);

      void push_structured_event(const CosNotification::StructuredEvent &_notification
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosEventComm::Disconnected);

      void analyzeTraffic(CORBA::ULong _timestamp, CORBA::ULong _size);

    private:
      //------------------------------------------------------------------------
      // private data
      //------------------------------------------------------------------------
      Parameters * parameters_;

      ACE_SOCK_Dgram_Mcast& socket_;
      std::string const domainName_;
      unsigned long int requestId_;

      EventFilter * event_filter_;

      CORBA::ULong lastTrafficAnalysis_;
      BandwidthQueue bandwidth_;

      //------------------------------------------------------------------------
      // private static constants
      //------------------------------------------------------------------------
      static CORBA::ULong const TIMEOUT_MSEC = 2000;
    };
  }
}

#endif // NotifyMulticastSender_h
