// -*- c++ -*- //////////////////////////////////////////////////////////////
//
//  NotifyMulticastReceiver
//
//
//  (c) 2002, 2004
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <phbaer@npw.net>
//    Hans Utz <hans.utz@neuro.informatik.uni-ulm.de>
//
//
//  Version:
//    1.1.0
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
#include "NotifyMulticastSH.h"

#include "Log.h"
#include "TimeHelper.h"

#include <iostream>
#include <ace/OS.h>

#include <tao/CDR.h>

#include <cstdlib>

namespace Miro 
{
  namespace NMC 
  {
    /**
     * Receiver::Receiver()
     *
     *   Description:
     *     Default constructor
     *
     *   Parameter:
     *     _configuration: Pointer to config object
     */
    Receiver::Receiver(ACE_SOCK_Dgram_Mcast& _socket,
		       CosNotifyChannelAdmin::EventChannel_ptr _ec,
		       std::string const& _domainName,
		       Parameters const * _params) :
      Super(_ec, _domainName),
      socket_(_socket),
      params_(_params),
      maxRetry_(5),
      mutex_(),
      localIPs_(),
      droppedLocal_(0) 
    {
      MIRO_LOG_CTOR("NMC::Receiver");
      
      shValid_ = false;
      
      ACE_INET_Addr *locals = NULL;
      size_t         localsCount;
      
      ACE::get_ip_interfaces(localsCount, locals);
      
      MIRO_DBG(NMC, LL_DEBUG, "Local ip interfaces determined by Receiver:");
      
      for (ACE_INET_Addr *i = locals; i != locals + localsCount; ++i) {
	
	struct in_addr ia;
	
	ia.s_addr = ACE_HTONL(i->get_ip_address());
	localIPs_.push_back(i->get_ip_address());
	
	MIRO_DBG_OSTR(NMC, LL_DEBUG,  "  " << inet_ntoa(ia));
      }
      std::sort(localIPs_.begin(), localIPs_.end());
      
      delete[] locals;
      
      connected_ = true;
    }
    
    
    /**
     * Receiver::~Receiver
     *
     *   Description:
     *     Default destructor
     */
    Receiver::~Receiver() 
    {
      MIRO_LOG_DTOR("NMC::Receiver");

      localIPs_.clear();
    }


    /**
     * Receiver::disconnect_structured_push_consumer()
     *
     *   Descritpion:
     *     Push consumer is disconnected
     */
    void
    Receiver::disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      throw(CORBA::SystemException)
    {
      MIRO_DBG(NMC, LL_DEBUG, "Receiver: push consumer disconnected!");

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
    void 
    Receiver::sendEvent(const CosNotification::StructuredEvent& _event)
      throw(CosEventComm::Disconnected) 
    {
      if (connected_) {

	try {
	  if (!CORBA::is_nil(proxyConsumer_.in()))
	    MIRO_DBG_OSTR(NMC, LL_PRATTLE,
			  "Receiver: received event: " <<
			  _event.header.fixed_header.event_type.domain_name << " " <<
			  _event.header.fixed_header.event_type.type_name);
	    proxyConsumer_->push_structured_event(_event);
	}
	catch (CORBA::Exception const&) {
	  connected_ = false;

	  MIRO_LOG(LL_ERROR, "NMC::Receiver::sendEvent(): cannot push structured event");

	  throw CosEventComm::Disconnected();
	}

      } 
      else
	throw CosEventComm::Disconnected();
    }


    /**
     * Receiver::handle_input()
     *
     *   Description:
     *     Called when input is available
     */
    int 
    Receiver::handle_input() 
    {
      if (connected_) {
	Miro::Guard guard(mutex_);
	EventData     eventData;
	iovec         iov[1];
	ACE_INET_Addr from;

	//		std::cout << "INPUT ::" << std::endl;
	switch (receiveData(iov, from)) {

	case -1:
	  MIRO_LOG(LL_ERROR, "NMC::Receiver::handleInput(): receiveData failed (-1)");
	  return -1;

	case 0:
	  MIRO_LOG(LL_ERROR, "NMC::Receiver::handleInput(): receiveData returned 0 bytes");
	  return 0;

	default:
	  /* fall through */
	  break;
	}

#ifdef DEBUG_TOOMUCH
	struct in_addr ia;
	ia.s_addr = htonl(from.get_ip_address());
	MIRO_DBG_OSTR(NMC, LL_PRATTLE ,
		      "Datagram from " << inet_ntoa(ia) << ":" << from.get_port_number());
#endif

	/* Check if paket was sent locally and if so, drop it */
	if (is_loopback(from)) {
	  droppedLocal_++;

	  if (droppedLocal_ % 100 == 99)
	    MIRO_LOG_OSTR(LL_NOTICE, 
			  "NMC::Receiver::handle_input(): dropped " << 
			  droppedLocal_ << " packets from myself");

	  if (droppedLocal_ == 0xffffffff)
	    MIRO_LOG(LL_NOTICE, 
		     "NMC::Receiver::handle_input(): number of locally dropped packets reset to 0");

	  return 0;
	}

	/* Process packet */
	TAO_InputCDR  headerCdr(reinterpret_cast<char const *>(iov[0].iov_base),
				NMC_HEADER_SIZE,
				reinterpret_cast<char const *>(iov[0].iov_base)[0]);

	eventData.systemTimestamp = ACE_OS::gettimeofday().msec();

	headerCdr.read_boolean(eventData.byteOrder);
	headerCdr.read_ulong(eventData.requestId);
	headerCdr.read_ulong(eventData.requestSize);
	headerCdr.read_ulong(eventData.fragmentSize);
	headerCdr.read_ulong(eventData.fragmentOffset);
	headerCdr.read_ulong(eventData.fragmentId);
	headerCdr.read_ulong(eventData.fragmentCount);
	headerCdr.read_ulong(eventData.timestamp);

	handle_event(from, eventData, 
		     reinterpret_cast<char const *>(iov[0].iov_base) + 
		     NMC_HEADER_SIZE, 
		     iov[0].iov_len - NMC_HEADER_SIZE);

	delete[] reinterpret_cast<char const *>(iov[0].iov_base);
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
    int 
    Receiver::handle_timeout(const ACE_Time_Value &/*_tv*/, const void */*_act*/)
    {
      if (!mutex_.tryacquire()) {

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
	  } 
	  else {
	    ++j;
	  }
	}
		
	mutex_.release();
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
    int 
    Receiver::handle_event(ACE_INET_Addr const& _from,
			   EventData const &_eventData, 
			   char const *_buffer, 
			   unsigned long _len) 
    {

      /* Drop the packet if data doesn't fit */

      if ((_eventData.requestSize    <  _eventData.fragmentSize) ||
	  (_eventData.fragmentOffset >= _eventData.requestSize)  ||
	  (_eventData.fragmentId     >= _eventData.fragmentCount)) {

	MIRO_LOG(LL_NOTICE, "NMC::Receiver: Dropping packet.");

	MIRO_DBG_OSTR(NMC, LL_DEBUG, 
		      "NMC::Receiver::handle_event(): Packet (id=" << 
		      _eventData.requestId << ") dropped due to errors:" <<
		      std::endl <<
		      "(requestSize >= fragmentSize) && (fragmentOffset < requestSize) && (fragmentId < fragmentCount)" << 
		      std::endl <<
		      "  requestSize   " << _eventData.requestSize << std::endl <<
		      "  fragmentSize  " << _eventData.fragmentSize << std::endl <<
		      "  fragmentId    " << _eventData.fragmentId << std::endl <<
		      "  fragmentCount " << _eventData.fragmentCount);

	return 0;
      }

      /* Index the incomplete (due to fragmentation) requests */
      RequestIndex     mapIndex(_from, _eventData.requestId);
      RequestMapEntry *entry;

      if (this->requestMap_.find(mapIndex, entry) == -1) {

	/* Create an entry and insert it... */
	RequestEntry *requestEntry = new RequestEntry(_eventData.byteOrder,
						      _eventData.requestId,
						      _eventData.requestSize,
						      _eventData.fragmentCount);

	/* Drop the packet if impossible to bind entry to map */

	if (this->requestMap_.bind(mapIndex, requestEntry, entry) == -1) {
	  MIRO_LOG(LL_NOTICE, "NMC::Receiver::handle_event(): Unable to index, dropping");

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
	MIRO_LOG(LL_NOTICE, "Fragment rejected");
	MIRO_DBG_OSTR(NMC, LL_DEBUG, 
		      "NMC::Receiver::handle_event(): Fragment (id=" <<
		      _eventData.fragmentId << ") rejected due to errors.");

	return 0;
      }

      /* Already received this fragment */
      /* WAS: "if (false && entry->...)" */
      if (entry->int_id_->testReceived(_eventData.fragmentId) == 1) {
	MIRO_LOG(LL_NOTICE, 
		 "NMC::Receiver::handle_event(): Duplicate fragment, dropping.");

	return 0;
      }

      /* Copy the payload into the fragment buffer */
      memcpy(entry->int_id_->fragmentBuffer(_eventData.fragmentOffset),
	     _buffer,
	     _len);

      /* Mark datagram fragment as received */
      entry->int_id_->markReceived(_eventData.fragmentId);

      /* If the message is not complete we must return... */
      if (!entry->int_id_->complete()) {
	MIRO_LOG(LL_NOTICE, 
		 "NMC::Receiver::handle_event(): message incomplete, message dropped.");

	return 0;
      }

      /* Demarshal datagram payload */
      try {
	CosNotification::StructuredEvent event;

	/* Decode datagram */
	entry->int_id_->decode(event);

	MIRO_DBG_OSTR(NMC, LL_PRATTLE,
		      "Receiver::handle_event(): Got event "
		      << event.header.fixed_header.event_type.domain_name
		      << " / "
		      << event.header.fixed_header.event_type.type_name); 

	/* pub/sub protocol hook */
	if (!strcmp(event.header.fixed_header.event_type.type_name, 
		    "NotifyMulticast::offered")) {
	  CosNotification::EventTypeSeq *ets;
	  event.remainder_of_body >>= ets;
	  if (shValid_) {
	    sh_->handleOffers(*ets);
	  }
	} 
	else if (!strcmp(event.header.fixed_header.event_type.type_name, 
			 "NotifyMulticast::subscribed")) {
	  CosNotification::EventTypeSeq *ets;
	  event.remainder_of_body >>= ets;
	  if (shValid_) {
	    sh_->handleSubscriptions(*ets);
	  }
	} 
	else {
                    
	  // Drop packet if it's too old
	  ACE_Time_Value now = ACE_OS::gettimeofday();
	  unsigned long nowmsec = now.msec();
	  unsigned long deltamsec = 
	    (nowmsec > _eventData.timestamp)? 
            nowmsec - _eventData.timestamp : 
            _eventData.timestamp - nowmsec;
          ACE_Time_Value delta;
          delta.msec(deltamsec);

	  if (params_->messageTimeout != ACE_Time_Value::zero &&
	      delta > params_->messageTimeout) {
	    MIRO_DBG_OSTR(NMC, LL_PRATTLE,
			  "NMC::Receiver::handle_event(): Packet " <<
			  _eventData.requestId <<
			  " dropped because it was too old: " << delta << "msec");
	  }
	  else {
	    /* Send event to event channel */
	    sendEvent(event);
	  }
	      
	  delete entry->int_id_;
	  this->requestMap_.unbind(entry);
	}
      } 
      catch (...) {
	MIRO_LOG(LL_ERROR, "NMC::Receiver::handle_event(): Exception");
      }

      return 0;
    }

    /**
     *     Reads data from the MC-Notification-Channel into an IOVec.
     *
     *   Parameters:
     *     _iov:    IOVec in which the data is stored
     *     _from:   Sender address
     *     _flags:  FLags for recv()
     */
    int
    Receiver::receiveData(iovec *_iov, ACE_INET_Addr &_from, int _flags) 
    {
      return socket_.recv(_iov, _from, _flags);
    }

    void Receiver::setSH(SH *_sh) 
    {
      if (_sh != NULL) {
	shValid_ = true;
	sh_ = _sh;
      }
    }

    void Receiver::invalidateSH() 
    {
      shValid_ = false;
    }
  }
}
