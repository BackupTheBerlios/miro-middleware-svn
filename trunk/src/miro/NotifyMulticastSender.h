// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticastSender
//
//
//  (c) 2002, 2003
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <philipp.baer@informatik.uni-ulm.de>
//
//
//  Version:
//    1.0.4
//
//
//  Description:
//
//   push_structured_event is called when new event is sent over the
//   EventChannel. Each event is serialized and sent to the peers
//   using ip multicast.
//
//
//  Changes:
//
//    1.0.4
//     - added some logging output
//
//    1.0.3
//      - many clean ups
//      - changed namespace, renamed class
//      - removed RtEC dependencies
//
//    1.0.1.experimental
//      - Changed interface
//
//    1.0.0.experimental
//      - Initial release, not tested!
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef NotifyMulticastSender_h
#define NotifyMulticastSender_h

/* TAO includes */
#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

/* ACE includes */
#include <ace/SOCK_Dgram_Mcast.h>

/* Miro includes */
#include <miro/StructuredPushConsumer.h>

namespace Miro {

    namespace NotifyMulticast {

        class Config;
        class Adapter;

        class Sender : public Miro::StructuredPushConsumer {
            typedef Miro::StructuredPushConsumer Super;

        public:
            /* Default constructor */
            Sender(Adapter *_main,
                   Config  *configuration_);

            /* Default destructor */
            ~Sender();

            friend class Adapter;

            int sendData(iovec *_iov, size_t _iovLen);

        protected:
            void subscribe(const std::string _domain, const std::string _type)
            throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

            void unsubscribe(const std::string _domain, const std::string _type)
            throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

            void marshalEvent(const CosNotification::StructuredEvent &_event);

            void sendCdrStream(TAO_OutputCDR &cdr);

            unsigned long int getFragmentCount(const ACE_Message_Block *_begin,
                                               const ACE_Message_Block *_end,
                                               int                      _iovSize,
                                               CORBA::ULong             _maxFragmentPayload,
                                               CORBA::ULong            &_totalLength);

            void sendFragment(CORBA::ULong         _requestId,
                              CORBA::ULong         _requestSize,
                              CORBA::ULong         _fragmentSize,
                              CORBA::ULong         _fragmentOffset,
                              CORBA::ULong         _fragmentId,
                              CORBA::ULong         _fragmentCount,
                              CORBA::ULong         _timestamp,
                              iovec                _iov[],
                              int                  _iovcnt);

            void disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
            throw(CORBA::SystemException);

            void push_structured_event(const CosNotification::StructuredEvent &_notification ACE_ENV_ARG_DECL_WITH_DEFAULTS)
            throw(CORBA::SystemException, CosEventComm::Disconnected);

            Adapter           *main_;
            Config            *configuration_;
            unsigned long int  requestId_;
        };
    };
};

#endif /* notifyForwardPushConsumer_hh */
