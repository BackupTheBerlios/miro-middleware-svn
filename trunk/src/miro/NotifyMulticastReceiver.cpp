// -*- c++ -*- //////////////////////////////////////////////////////////////
//
//  NotifyMulticastReceiver
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
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

/* NotifyMulticast includes */
#include "NotifyMulticastDefines.h"
#include "NotifyMulticastReceiver.h"
#include "NotifyMulticastAdapter.h"

/* c++lib includes */
// #include <stream>

#define DBG_CLASSNAME "NotifyMulticast::Receiver"

namespace Miro {

    namespace NotifyMulticast {

        /**
         * Receiver::Receiver()
         *
         *   Description:
         *     Default constructor
         *
         *   Parameter:
         *     _main:          Pointer to NotifyMulticast adapter
         *     _configuration: Pointer to config object
         */
        Receiver::Receiver(Adapter *_main,
                           Config  *_configuration) :
            Super(_configuration->getEventchannel(), _configuration->getDomain()),
            main_(_main),
            configuration_(_configuration),
            maxRetry_(5),
            mutex_(),
            localIPs_()
        {
            PRINT_DBG(DBG_INFO, "Initializing for domain " << configuration_->getDomain());

            ACE_INET_Addr *locals;
            size_t         localsCount;

            ACE::get_ip_interfaces(localsCount, locals);

            for (ACE_INET_Addr *i = locals; i != locals + localsCount; ++i)
                localIPs_.insert(i->get_ip_address());

            delete []locals;

            PRINT_DBG(DBG_MORE, "Initialized");
        }


        /**
         * Receiver::~Receiver
         *
         *   Description:
         *     Default destructor
         */
        Receiver::~Receiver() {
            PRINT_DBG(DBG_INFO, "Cleaning up");

            localIPs_.clear();

            PRINT_DBG(DBG_MORE, "Cleaned up");
        }


        /**
         * Receiver::disconnect_structured_push_consumer()
         *
         *   Descritpion:
         *     Push consumer is disconnected
         */
        void Receiver::disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
            throw(CORBA::SystemException)
        {
            PRINT("Disconnected");

            connected_ = false;
        }


        /**
         * Receiver::sendEvent()
         *
         *   Description:
         *     Sends a structured event throught the MC Notification Channel to
         *     the audience.
         *
         *   Parameters:
         *     event: Structured event that should be sent
         */
        void Receiver::sendEvent(const CosNotification::StructuredEvent& event)
            throw(CosEventComm::Disconnected)
        {
            if (connected_) {
                Miro::Guard guard(connectedMutex_);

                try {
                    if (!CORBA::is_nil(proxyConsumer_.in()))
                        proxyConsumer_->push_structured_event(event);
                } catch (...) {
                    connected_ = false;
                    throw CosEventComm::Disconnected();
                }

            } else
                throw CosEventComm::Disconnected();
        }


        /**
         * Receiver::handle_input()
         *
         *   Description:
         *     Called when input is available
         */
        int Receiver::handle_input() {
            if (connected_) {
                // TODO: Check if really needed! (Try not to read all data from socket)
                Miro::Guard   guard(mutex_); /* needed for timeout handling */
                CORBA::ULong  header[HEADER_SIZE / sizeof(CORBA::ULong) + ACE_CDR::MAX_ALIGNMENT];
                EventData     eventData;
                iovec         iov[1];
                ACE_INET_Addr from;

                switch (receiveData(iov, from)) {
                case -1:
                    PRINT_DBG(DBG_INFO, "handleInput: read");
                    return -1;

                case 0:
                    PRINT_DBG(DBG_INFO, "handleInput: read 0");
                    return 0;

                default:
                    /* fall through */
                    break;
                }

                /* Check if paket was sent locally and if so, drop it */
                if (is_loopback(from))
                    return 0;

//#if DEBUG_LEVEL == DBG_TOOMUCH
                struct in_addr ia;
                ia.s_addr = htonl(from.get_ip_address());

                PRINT("Datagram from " << inet_ntoa(ia) << ":" << from.get_port_number());
//#endif

                /* Process packet */
                memcpy(header, ((char *)iov[0].iov_base ), sizeof(header));

                char         *buf = ACE_reinterpret_cast(char *, header);
                TAO_InputCDR  headerCdr(buf, sizeof(header), (int)buf[0]);

                eventData.systemTimestamp = ACE_OS::gettimeofday().msec();
                headerCdr.read_boolean(eventData.byteOrder);
                headerCdr.read_ulong(eventData.requestId);
                headerCdr.read_ulong(eventData.requestSize);
                headerCdr.read_ulong(eventData.fragmentSize);
                headerCdr.read_ulong(eventData.fragmentOffset);
                headerCdr.read_ulong(eventData.fragmentId);
                headerCdr.read_ulong(eventData.fragmentCount);
                headerCdr.read_ulong(eventData.timestamp);

                cout << "test" << endl;
                handle_event(eventData, iov);
            }

            return 0;
        }


        /**
         * Receiver::handle_timeout()
         *
         *   Description:
         *     Called by TimeoutHandler to clean fragment database.
         *     Code is copied character-by-character from
         *     ACE_wrappers/TAO/orbsvcs/orbsvcs/Event/EC_Gateway_UDP.cpp.
         *     I tried to write my own timout routine, but i didn't
         *     succeed.
         */
        int Receiver::handle_timeout(const ACE_Time_Value &/*_tv*/, const void */*_act*/) {
            // TODO: Check if mutex is really needed!
            Miro::Guard guard(mutex_);

            RequestMap::iterator begin = requestMap_.begin();
            RequestMap::iterator end = requestMap_.end();

            {
                for (RequestMap::iterator i = begin; i != end; ++i) {
                    (*i).int_id_->incTimeout();
                }
            }

            for (RequestMap::iterator j = begin; j != end; ) {
                if ((*j).int_id_->getTimeout() > this->maxRetry_) {
                    RequestMapEntry& entry = *j;
                    ++j;
                    {
                        delete entry.int_id_;
                        this->requestMap_.unbind(&entry);
                    }
                } else {
                    ++j;
                }
            }

            return 0;
        }


        /**
         * Receiver::handle_event()
         *
         *   Description:
         *     Processes an event that arrived through the MC-Channel
         *
         *   Parameters:
         *     _eventData: header of IP/MC Event packet
         *     _iov:       Complete packet
         */
        int Receiver::handle_event(EventData &_eventData, iovec *_iov) {
            ACE_INET_Addr from;

            /* Drop the packet if data doesn't fit */
            if ((_eventData.requestSize    <  _eventData.fragmentSize) ||
                (_eventData.fragmentOffset >= _eventData.requestSize)  ||
                (_eventData.fragmentId     >= _eventData.fragmentCount)) {

                PRINT_DBG(DBG_INFO, "Dropping packet");

                return 0;
            }

            /* Index the incomplete (due to fragmentation) requests */
            RequestIndex     mapIndex(from, _eventData.requestId);
            RequestMapEntry *entry;

            if (this->requestMap_.find(mapIndex, entry) == -1) {

                /* Create an entry and insert it... */
                RequestEntry *requestEntry = new RequestEntry(_eventData.byteOrder,
                                                              _eventData.requestId,
                                                              _eventData.requestSize,
                                                              _eventData.fragmentCount);

                /* Drop the packet if impossible to bind entry to map */
                if ((requestEntry == 0) || (this->requestMap_.bind(mapIndex, requestEntry, entry) == -1)) {
                    PRINT_DBG(DBG_VERBOSE, "Unable to index, dropping");

                    return 0;
                }
            }

            /* Validate the message... */
            if (entry->int_id_->validateFragment(_eventData.byteOrder,
                                                 _eventData.requestSize,
                                                 _eventData.fragmentSize,
                                                 _eventData.fragmentOffset,
                                                 _eventData.fragmentId,
                                                 _eventData.fragmentCount) == 0) {
                PRINT_DBG(DBG_VERBOSE, "Fragment rejected");

                return 0;
            }

            /* Already received this fragment */
            if (entry->int_id_->testReceived(_eventData.fragmentId) == 1) {
                PRINT_DBG(DBG_VERBOSE, "Duplicate Fragment, dropping");

                return 0;
            }

            /* Copy the payload into the fragment buffer */
            char *buffer = (char *)_iov[0].iov_len + HEADER_SIZE;
            int   bufferLen = _iov[0].iov_len - HEADER_SIZE;

            memcpy(entry->int_id_->fragmentBuffer(_eventData.fragmentOffset),
                   buffer,
                   bufferLen);

            /* Mark datagram fragment as received */
            entry->int_id_->markReceived(_eventData.fragmentId);

            /* If the message is not complete we must return... */
            if (!entry->int_id_->complete())
                return 0;

            /* Demarshal datagram payload */
            ACE_DECLARE_NEW_CORBA_ENV;
            ACE_TRY {
                CosNotification::StructuredEvent event;

                /* Decode datagram */
                entry->int_id_->decode(event ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;

                /* Drop packet if it's too old */
                if (ACE_OS::gettimeofday().msec() - _eventData.timestamp > configuration_->getEventMaxAge()) {
                    PRINT_DBG(DBG_VERBOSE, "Packet " << _eventData.requestId << " dropped because it was too old");
                    return 0;
                }

                /* Send event to event channel */
                sendEvent(event ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK;

            } ACE_CATCHANY {
                ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "NotifyMulticastReceiver::handle_input");
            }
            ACE_ENDTRY;

            return 0;
        }


        /**
         * Receiver::isLoopback()
         *
         *   Description:
         *     Check if address (_from) is local
         *
         *   Parameters:
         *     _from: address to check
         */
        bool Receiver::is_loopback(const ACE_INET_Addr &_from) {
            return (localIPs_.find(_from.get_ip_address()) != localIPs_.end());
        }


        /**
         * Receiver::receiveData()
         *
         *   Description:
         *     Reads data from the MC-Notification-Channel into an IOVec.
         *
         *   Parameters:
         *     _iov:    IOVec in which the data is stored
         *     _iovLen: Length (size) of _iov
         *     _from:   Sender address
         *     _flags:  FLags for recv()
         */
        int Receiver::receiveData(iovec *_iov, int _iovLen, ACE_INET_Addr &_from, int _flags) {
            return configuration_->getSocket()->recv(_iov, _iovLen, _from, _flags);
        }


        /**
         * Receiver::receiveData()
         *
         *   Description:
         *     Reads data from the MC-Notification-Channel into an IOVec.
         *
         *   Parameters:
         *     _iov:    IOVec in which the data is stored
         *     _from:   Sender address
         *     _flags:  FLags for recv()
         */
        int Receiver::receiveData(iovec *_iov, ACE_INET_Addr &_from, int _flags) {
            return configuration_->getSocket()->recv(_iov, _from, _flags);
        }


        /**
         * Receiver::receiveData()
         *
         *   Description:
         *     Reads data from the MC-Notification-Channel into a buffer.
         *
         *   Parameters:
         *     _data:    Pointer to a buffer
         *     _dataLen: Size of the buffer
         *     _from:    Sender address
         *     _flags:   FLags for recv()
         */
        int Receiver::receiveData(void *_data, int _dataLen, ACE_INET_Addr &_from, int _flags) {
            return configuration_->getSocket()->recv(_data, _dataLen, _from, _flags);
        }

    };
};
