// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticastSender
//
//
//  (c) 2002
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <phbaer@openums.org>
//
//
//  Version:
//    1.0.3
//
//
//  Todo:
//    - Subscription vector
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

/* NotifyMulticast includes */
#include "NotifyMulticastDefines.h"
#include "NotifyMulticastSender.h"
#include "NotifyMulticastAdapter.h"
#include "NotifyMulticastParameters.h"

/* ACE includes */
#include <ace/Message_Block.h>

/* g++lib includes */
// #include <stream>

#define DBG_CLASSNAME "NotifyMulticast::Sender"

namespace Miro {

    namespace NotifyMulticast {

        /**
         * Sender::Sender()
         *
         *   Descritpion:
         *     Default constructor
         *
         *   Parameters:
         *     _main:          Pointer to adapter instance
         *     _configuration: Pointer to configuration instance
         */
        Sender::Sender(Adapter *_main,
                       Config  *_configuration) :
            Super(_configuration->getEventchannel()),
            main_(_main),
            configuration_(_configuration),
            requestId_(0)
        {
            PRINT_DBG(DBG_INFO, "Initializing");

	    try {
		Parameters *parameters = Parameters::instance();

                // TODO: Subscription changing

		for (std::set<std::string>::iterator itr = parameters->subscription.begin();
		     itr != parameters->subscription.end();
		     itr++)
                    subscribe(configuration_->getDomain(), *itr);
		//subscribe(configuration_->getDomain(), "BallPosition");
                //subscribe(configuration_->getDomain(), "LocalizedPosition");
            } catch (const CORBA::SystemException &e) {
                PRINT_ERR("Unable to subscribe events: " << e);
                PRINT_ERR("No events will be sent to the peers");
            } catch (const CosNotifyComm::InvalidEventType &e) {
                PRINT_ERR("Unable to subscribe events: " << e);
                PRINT_ERR("No events will be sent to the peers");
            } catch (...) {
                PRINT_ERR("Unable to subscribe events: uncaught exception");
                PRINT_ERR("No events will be sent to the peers");
            }

            PRINT_DBG(DBG_MORE, "Initialized");
        }


        /**
         * Sender::~Sender()
         *
         *   Description:
         *     Default destructor
         */
        Sender::~Sender() {
            PRINT_DBG(DBG_INFO, "Cleaning up");

            PRINT_DBG(DBG_MORE, "Cleaned up");
        }


        /**
         * Sender::subscribe()
         *
         *   Descritpion:
         *     Subscribe events from _domain of type _type
         *
         *   Parameters:
         *     _domain: domain of events that are going to be subscribed
         *     _type:   type of event that are going to be subscribed
         */
        void Sender::subscribe(const std::string _domain, const std::string _type)
            throw(CORBA::SystemException, CosNotifyComm::InvalidEventType)
        {
            /* change subscription */
            CosNotification::EventTypeSeq added(1);
            CosNotification::EventTypeSeq removed(0);
            added.length(1);
            removed.length(1);
            added[0].domain_name = CORBA::string_dup(_domain.c_str());
            added[0].type_name   = CORBA::string_dup(_type.c_str());
            removed[0].domain_name = CORBA::string_dup("*");
            removed[0].type_name   = CORBA::string_dup("*");
            consumerAdmin_->subscription_change(added, removed);
        }


        /**
         * Sender::unsubscribe()
         *
         *   Descritpion:
         *     Unsubscribe events from _domain of type _type
         *
         *   Parameters:
         *     _domain: domain of events that are going to be unsubscribed
         *     _type:   type of events that are going to be unsubscribed
         */
        void Sender::unsubscribe(const std::string _domain, const std::string _type)
            throw(CORBA::SystemException, CosNotifyComm::InvalidEventType)
        {
            /* change subscription */
            CosNotification::EventTypeSeq added(0);
            CosNotification::EventTypeSeq removed(1);
            added.length(0);
            removed.length(1);
            removed[0].domain_name = CORBA::string_dup(_domain.c_str());
            removed[0].type_name   = CORBA::string_dup(_type.c_str());
            consumerAdmin_->subscription_change(added, removed);
        }


        /**
         * Sender::push_structued_event()
         *
         *   Description:
         *     Method from CosNotification that must be overwritten
         *     Event will be marshalled, if is from local
         *
         *   Parameters:
         *     _notification: Pointer to event object
         */
        void Sender::push_structured_event(const CosNotification::StructuredEvent &_notification
                                           ACE_ENV_ARG_DECL_NOT_USED)
            throw(CORBA::SystemException, CosEventComm::Disconnected)
        {
            /* Where is the event comming from? */
            if (!ACE_OS::strcmp((const char *)_notification.header.fixed_header.event_type.domain_name, configuration_->getDomain().c_str()))
                marshalEvent(_notification);
        }


        /**
         * Sender::disconnect_push_consumer()
         *
         *   Descritpion:
         *     Method that is called by the eventchannel when a consumer was disconnected
         */
        void Sender::disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        throw(CORBA::SystemException) {
            PRINT_DBG(DBG_INFO, "Consumer disconnected");

            connected_ = false;
        }


        /**
         * Sender::marshalEvent
         *
         *   Descritpion:
         *     Marshals a given event
         *
         *   Parameters:
         *     _event: Event that has to be marshalled
         */
        void Sender::marshalEvent(const CosNotification::StructuredEvent &_event) {
            TAO_OutputCDR cdr;

            /* marschal event */
            if (!(cdr << _event)) {
                PRINT_ERR("Unable to marshal event, discarding!");
                return;
            }

            sendCdrStream(cdr);
        }


        /**
         * Sender::sendCdrStream()
         *
         *   Description:
         *     Takes a CDR stream as the only argument and tried to send it over the
         *     NotificationMulticastChannel. The CDR data is splitted into several fragments
         *     if needed.
         *
         *   Parameters:
         *     _cdr: CORBA CDR Stream
         */
        void Sender::sendCdrStream(TAO_OutputCDR &_cdr) {
            CORBA::ULong maxFragmentPayload = DEFAULT_MTU - HEADER_SIZE;

            #if defined (ACE_HAS_BROKEN_DGRAM_SENDV)

            const int TAO_WRITEV_MAX = IOV_MAX - 1;
            #else

            const int TAO_WRITEV_MAX = IOV_MAX;
            #endif

            iovec iov[TAO_WRITEV_MAX];

            CORBA::ULong totalLength = 0;
            CORBA::ULong fragmentCount = getFragmentCount(_cdr.begin(),
                                                          _cdr.end(),
                                                          TAO_WRITEV_MAX,
                                                          maxFragmentPayload,
                                                          totalLength);

            CORBA::ULong timestamp = ACE_OS::gettimeofday().msec();

            requestId_++;

            int          iovcnt = 1;
            CORBA::ULong fragmentId = 0;
            CORBA::ULong fragmentOffset = 0;
            CORBA::ULong fragmentSize = 0;

            for (const ACE_Message_Block* b = _cdr.begin(); b != _cdr.end(); b = b->cont()) {
                CORBA::ULong  len = b->length();
                char         *rdPtr = b->rd_ptr();

                iov[iovcnt].iov_base = rdPtr;
                iov[iovcnt].iov_len  = len;
                iovcnt++;

                fragmentSize += len;

                /* This fragment is full, we have to send it... */
                while (fragmentSize > maxFragmentPayload) {

                    /* First adjust the last iov entry: */
                    unsigned long int lastMbLength =  maxFragmentPayload - (fragmentSize - len);
                    iov[iovcnt - 1].iov_len = lastMbLength;

                    sendFragment(requestId_,
                                 totalLength,
                                 maxFragmentPayload,
                                 fragmentOffset,
                                 fragmentId,
                                 fragmentCount,
                                 timestamp,
                                 iov,
                                 iovcnt);
                    ACE_CHECK;

                    fragmentId++;
                    fragmentOffset += maxFragmentPayload;

                    /* Reset, but don't forget that the last Message_Block
                     * may need to be sent in multiple fragments.. */
                    len -= lastMbLength;
                    rdPtr += lastMbLength;
                    iov[1].iov_base = rdPtr;
                    iov[1].iov_len = len;
                    fragmentSize = len;
                    iovcnt = 2;
                }

                /* A fragment was filled exactly */
                if (fragmentSize == maxFragmentPayload) {
                    sendFragment(requestId_,
                                 totalLength,
                                 maxFragmentPayload,
                                 fragmentOffset,
                                 fragmentId,
                                 fragmentCount,
                                 timestamp,
                                 iov,
                                 iovcnt);
                    ACE_CHECK;

                    fragmentId++;
                    fragmentOffset += maxFragmentPayload;

                    iovcnt = 1;
                    fragmentSize = 0;
                }

                /* We ran out of space in the iovec, send the fragment */
                if (iovcnt == TAO_WRITEV_MAX) {
                    sendFragment(requestId_,
                                 totalLength,
                                 fragmentSize,
                                 fragmentOffset,
                                 fragmentId,
                                 fragmentCount,
                                 timestamp,
                                 iov,
                                 iovcnt);
                    ACE_CHECK;

                    fragmentId++;
                    fragmentOffset += fragmentSize;

                    iovcnt = 1;
                    fragmentSize = 0;
                }
            }

            /* There is something left in the IOVec that we must send also... */
            if (iovcnt != 1) {
                sendFragment(requestId_,
                             totalLength,
                             fragmentSize,
                             fragmentOffset,
                             fragmentId,
                             fragmentCount,
                             timestamp,
                             iov,
                             iovcnt);
                ACE_CHECK;

                fragmentId++;
                fragmentOffset += fragmentSize;

                /* reset not needed here... */
            }
        }


        /**
         * Sender::getFragementCount()
         *
         *   Descritpion:
         *     Computes the number of fragments needed to send the data over the network
         *
         *   Parameters:
         *     _begin:              Pointer to ACE_Message_Block start
         *     _end:                Pointer to ACE_Message_Block end
         *     _iovSize:            Size of IOVec (|_end - _begin|)
         *     _maxFragmentPeyload: Max. payload per fragment
         *     _totalLength:        Length of data
         */
        unsigned long int Sender::getFragmentCount(const ACE_Message_Block *_begin,
                                                   const ACE_Message_Block *_end,
                                                   int                      _iovSize,
                                                   CORBA::ULong             _maxFragmentPayload,
                                                   CORBA::ULong            &_totalLength) {
            CORBA::ULong fragmentCount = 0;
            CORBA::ULong fragmentSize = 0;

            _totalLength = 0;

            /* Reserve the first iovec for the header... */
            int iovcnt = 1;
            for (const ACE_Message_Block* b = _begin; b != _end; b = b->cont ()) {
                CORBA::ULong len = b->length();

                _totalLength += len;
                fragmentSize += len;

                iovcnt++;

                /* Ran out of space, create fragment... */
                while (fragmentSize > _maxFragmentPayload) {
                    fragmentCount++;

                    /* The next iovector will contain what remains of this buffer */
                    iovcnt = 2;

                    len -= _maxFragmentPayload - (fragmentSize - len);

                    fragmentSize = len;
                }

                if (fragmentSize == _maxFragmentPayload) {
                    fragmentCount++;
                    iovcnt = 1;
                    fragmentSize = 0;
                }

                /* Ran out of space in the iovector... */
                if (iovcnt >= _iovSize) {
                    fragmentCount++;
                    iovcnt = 1;
                    fragmentSize = 0;
                }
            }

            /* Send the remaining data in another fragment */
            if (iovcnt != 1)
                fragmentCount++;

            return fragmentCount;
        }


        /**
         * Sender::sendFragment()
         *
         *   Descritpion:
         *     Sends fragment over the network
         *
         *   Parameters:
         *     _requestId:      id of this request
         *     _requestSize:    Size of this request
         *     _fragmentSize:   Size of this fragment
         *     _fragmentOffset: Fragement offset
         *     _fragmentId:     Fragment id
         *     _fragmentCount:  Fragment count
         *     _iovec:          Data that is going to be sent
         *     _iovcnt:         Number of elements in IOVec
         */
        void Sender::sendFragment(CORBA::ULong _requestId,
                                  CORBA::ULong _requestSize,
                                  CORBA::ULong _fragmentSize,
                                  CORBA::ULong _fragmentOffset,
                                  CORBA::ULong _fragmentId,
                                  CORBA::ULong _fragmentCount,
                                  CORBA::ULong _timestamp,
                                  iovec        _iov[],
                                  int          _iovcnt) {

            CORBA::ULong header[HEADER_SIZE / sizeof(CORBA::ULong) + ACE_CDR::MAX_ALIGNMENT];

            char          *buf = ACE_reinterpret_cast(char *, header);
            TAO_OutputCDR  cdr(buf, sizeof(header));

            /* create header */
            cdr.write_boolean(TAO_ENCAP_BYTE_ORDER);
            cdr.write_ulong(_requestId);
            cdr.write_ulong(_requestSize);
            cdr.write_ulong(_fragmentSize);
            cdr.write_ulong(_fragmentOffset);
            cdr.write_ulong(_fragmentId);
            cdr.write_ulong(_fragmentCount);
            cdr.write_ulong(_timestamp);                 /* timestamp */

            PRINT_DBG(DBG_TOOMUCH,
                      "Event Datagram Sent" << endl <<
                      "  request id:      " << _requestId << endl <<
                      "  request size:    " << _requestSize << endl <<
                      "  fragment size:   " << _fragmentSize << endl <<
                      "  fragment offset: " << _fragmentOffset << endl <<
                      "  fragment id:     " << _fragmentId << endl <<
                      "  fragment count:  " << _fragmentCount << endl <<
                      "  timestamp:       " << _timestamp << endl);

            _iov[0].iov_base = cdr.begin()->rd_ptr();
            _iov[0].iov_len  = cdr.begin()->length();

            switch (sendData(_iov, _iovcnt)) {
            case -1:
                PRINT_DBG(DBG_INFO, "send_fragment: send failed");
                throw CORBA::COMM_FAILURE();
                break;

            case 0:
                PRINT_DBG(DBG_INFO, "send_fragment: EOF on send");
                throw CORBA::COMM_FAILURE();
                break;

            default:
                break;
            }

        }


        /**
         * Sender::sendData()
         *
         *   Description:
         *     Send data
         *
         *   Parameters:
         *     _iov:    IOVec
         *     _iovLen: Number of elements in IOVec
         */
        int Sender::sendData(iovec *_iov, size_t _iovLen) {
            return configuration_->getSocket()->send(_iov, (int)_iovLen);
        }
    };
};

