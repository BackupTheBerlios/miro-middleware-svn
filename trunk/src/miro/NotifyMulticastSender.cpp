// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticastSender
//
//
//  (c) 2002, 2003, 2004
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Hans Utz <hans.utz@informatik.uni-ulm.de
//    Philipp Baer <phbaer@npw.net>
//
//
//  Version:
//    1.1.0
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
#include "NotifyMulticastDomainEventFilter.h"

#include "TimeHelper.h"
#include "Log.h"

/* ACE includes */
#include <ace/Message_Block.h>
#include <ace/OS.h>

namespace Miro 
{
  namespace NMC 
  {
    using ::operator<<;

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
		   Config *_configuration) :
      Super(_configuration->getEventchannel()),
      main_(_main),
      configuration_(_configuration),
      requestId_(0),
      lastTrafficAnalysis_(0)
    {
      MIRO_LOG_CTOR("NMC::Sender");

      event_filter_ = new DomainEventFilter(configuration_->getDomain());
    }


    /**
     * Sender::~Sender()
     *
     *   Description:
     *     Default destructor
     */
    Sender::~Sender() 
    {
      MIRO_LOG_DTOR("NMC::Sender");

      delete event_filter_;
    }


    void 
    Sender::init() 
    {
      try {
	Parameters *parameters = Parameters::instance();
	
	// TODO: Subscription changing
	
	std::set<std::string>::const_iterator first, last;
	first = parameters->subscription.begin();
	last = parameters->subscription.end();
	for (; first != last; ++first) {
	  subscribe(configuration_->getDomain(), *first);
	}
      } 
      catch (const CosNotifyComm::InvalidEventType &e) {
	MIRO_LOG_OSTR(LL_ERROR, 
		      "NMC::Sender: Unable to subscribe to events:" << e <<
		      "\nNo events will be sent to the peers.");
      }
      catch (const CORBA::Exception &e) {
	MIRO_LOG_OSTR(LL_ERROR, 
		      "NMC::Sender: Unable to subscribe to events:" << e <<
		      "\nNo events will be sent to the peers.");
      } 
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
    void 
    Sender::subscribe(std::string const& _domain,
			   std::string const& _type)
      throw(CORBA::SystemException, CosNotifyComm::InvalidEventType) {
      /* change subscription */
      CosNotification::EventTypeSeq added(1);
      CosNotification::EventTypeSeq removed(0);
      added.length(1);
      added[0].domain_name = CORBA::string_dup(_domain.c_str());
      added[0].type_name   = CORBA::string_dup(_type.c_str());

      consumerAdmin_->subscription_change(added, removed ACE_ENV_ARG_PARAMETER);
      MIRO_DBG_OSTR(NMC, LL_DEBUG, 
		    "NMC::Sender: Subscribed to domain=" << _domain << ", type=" << _type);
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
    void 
    Sender::unsubscribe(std::string const& _domain,
			std::string const& _type)
      throw(CORBA::SystemException, CosNotifyComm::InvalidEventType) 
    {
      /* change subscription */
      CosNotification::EventTypeSeq added(0);
      CosNotification::EventTypeSeq removed(1);
      removed.length(1);
      removed[0].domain_name = CORBA::string_dup(_domain.c_str());
      removed[0].type_name   = CORBA::string_dup(_type.c_str());

      consumerAdmin_->subscription_change(added, removed ACE_ENV_ARG_PARAMETER);
      MIRO_DBG_OSTR(NMC, LL_DEBUG, 
		    "NMC::Sender: Unsubscribed to domain=" << _domain << ", type=" << _type);      
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
    void 
    Sender::push_structured_event(const CosNotification::StructuredEvent &_notification
				  ACE_ENV_ARG_DECL_NOT_USED)
      throw(CORBA::SystemException, CosEventComm::Disconnected) 
    {
      /* Where is the event coming from? */
      if (event_filter_->isAccepted(_notification)) {
	MIRO_DBG_OSTR(NMC, LL_PRATTLE,
		      "Sender: sending event: " <<
		      _notification.header.fixed_header.event_type.domain_name << " " <<
		      _notification.header.fixed_header.event_type.type_name);
	marshalEvent(_notification);
      }
    }


    /**
     * Sender::disconnect_push_consumer()
     *
     *   Description:
     *     Method that is called by the eventchannel when a consumer was disconnected
     */
    void
    Sender::disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      throw(CORBA::SystemException) 
    {
      MIRO_DBG(NMC, LL_DEBUG, "NMC::Sender: Consumer disconnected!");

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
    void
    Sender::marshalEvent(const CosNotification::StructuredEvent &_event) 
    {
      TAO_OutputCDR cdr;

      /* marschal event */

      if (cdr << _event) {
	sendCdrStream(cdr);
	return;
      }

      MIRO_LOG(LL_ERROR, "NMC::Sender: Unable to marshal event, discarding!");
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
    void
    Sender::sendCdrStream(TAO_OutputCDR &_cdr) 
    {
      CORBA::ULong maxFragmentPayload = NMC_DEFAULT_MTU - NMC_HEADER_SIZE;

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

      int iovcnt = 1;
      CORBA::ULong fragmentId = 0;
      CORBA::ULong fragmentOffset = 0;
      CORBA::ULong fragmentSize = 0;

      for (const ACE_Message_Block* b = _cdr.begin();
	   b != _cdr.end();
	   b = b->cont()) {
	CORBA::ULong  len = b->length();
	char *rdPtr = b->rd_ptr();

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

      /* There is something left in the IOVec that we must send, too... */
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
    unsigned long int
    Sender::getFragmentCount(const ACE_Message_Block *_begin,
			     const ACE_Message_Block *_end,
			     int _iovSize,
			     CORBA::ULong _maxFragmentPayload,
			     CORBA::ULong &_totalLength) 
    {
      CORBA::ULong fragmentCount = 0;
      CORBA::ULong fragmentSize = 0;

      _totalLength = 0;

      /* Reserve the first iovec for the header... */
      int iovcnt = 1;

      for (const ACE_Message_Block* b = _begin;
	   b != _end;
	   b = b->cont ()) {
	CORBA::ULong len = b->length();

	_totalLength += len;
	fragmentSize += len;

	iovcnt++;

	/* Ran out of space, new fragment... */

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
      if (iovcnt != 1) {
	fragmentCount++;
      }

      return fragmentCount;
    }


    /**
     * Sender::sendFragment()
     *
     *   Description:
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
    void 
    Sender::sendFragment(CORBA::ULong _requestId,
			 CORBA::ULong _requestSize,
			 CORBA::ULong _fragmentSize,
			 CORBA::ULong _fragmentOffset,
			 CORBA::ULong _fragmentId,
			 CORBA::ULong _fragmentCount,
			 CORBA::ULong _timestamp,
			 iovec _iov[],
			 int _iovcnt) 
    {
      CORBA::ULong header[NMC_HEADER_SIZE / sizeof(CORBA::ULong) + ACE_CDR::MAX_ALIGNMENT];
      char *buf = ACE_reinterpret_cast(char *, header);
      TAO_OutputCDR cdr(buf, sizeof(header));

      /* create header */
      cdr.write_boolean(TAO_ENCAP_BYTE_ORDER);
      cdr.write_ulong(_requestId);
      cdr.write_ulong(_requestSize);
      cdr.write_ulong(_fragmentSize);
      cdr.write_ulong(_fragmentOffset);
      cdr.write_ulong(_fragmentId);
      cdr.write_ulong(_fragmentCount);
      cdr.write_ulong(_timestamp);                 /* timestamp */

      MIRO_DBG_OSTR(NMC, LL_PRATTLE,
		"Event Datagram Sent" << std::endl <<
		"  request id:      " << _requestId << std::endl <<
		"  request size:    " << _requestSize << std::endl <<
		"  fragment size:   " << _fragmentSize << std::endl <<
		"  fragment offset: " << _fragmentOffset << std::endl <<
		"  fragment id:     " << _fragmentId << std::endl <<
		"  fragment count:  " << _fragmentCount << std::endl <<
		"  timestamp:       " << _timestamp << std::endl);

      _iov[0].iov_base = cdr.begin()->rd_ptr();
      _iov[0].iov_len  = cdr.begin()->length();

      size_t len = sendData(_iov, _iovcnt);
      switch (len) {
      case -1: 
	{
	  MIRO_LOG_OSTR(LL_ERROR, 
			"NMC::Sender: send_fragment: send failed (" <<
			strerror(errno) << ")");
	  throw CORBA::COMM_FAILURE();
	}
	break;

      case 0:
	{
	  MIRO_LOG(LL_ERROR, "NMC::Sender: send_fragment: EOF on send (0)");
	  throw CORBA::COMM_FAILURE();
	}
	break;

      default:
	analyzeTraffic(_timestamp, len);
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
    int 
    Sender::sendData(iovec *_iov, size_t _iovLen) 
    {
      ACE_Time_Value start = ACE_OS::gettimeofday();
      int rc =  configuration_->getSocket()->send(_iov, (int)_iovLen);
      ACE_Time_Value stop = ACE_OS::gettimeofday();

      MIRO_DBG_OSTR(NMC, LL_PRATTLE, "Sender: time for sending=" << (stop - start));
      return rc;
    }
	
    /** Set an Object for event filtering.
     *
     *  The object must be allocated dynamically (via new) and
     *  will be released (free'd) on the next call to either this
     *  function or the destructor, whichever comes first.
     */ 
    void 
    Sender::setEventFilter(EventFilter * _event_filter)
    {
      MIRO_ASSERT(_event_filter != 0);
      delete event_filter_;
      event_filter_ = _event_filter;
    }

    void
    Sender::analyzeTraffic(CORBA::ULong _timestamp, CORBA::ULong _size) {
      bandwidth_.push_back(std::make_pair(_timestamp, _size));

      while (_timestamp - bandwidth_.front().first > TIMEOUT_MSEC) {
	bandwidth_.pop_front();
      }

      if (_timestamp - lastTrafficAnalysis_ > 1000) {
	lastTrafficAnalysis_ = _timestamp;

	BandwidthQueue::const_iterator first, last = bandwidth_.end();
	CORBA::ULong sum = 0;
	for (first = bandwidth_.begin(); first != last; ++first) {
	  sum += first->second;
	}
	sum /= TIMEOUT_MSEC / 1000;


	MIRO_DBG_OSTR(NMC, LL_DEBUG, "Sender: output per second = " << sum);
      }
    }
  }
}

