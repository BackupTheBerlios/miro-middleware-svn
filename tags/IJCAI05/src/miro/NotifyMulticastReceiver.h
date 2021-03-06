// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast Receiver
//
//
//  (c) 2002, 2003, 2004
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
//  Description:
//
//    Supplier for an EventChannel. It is calles by NotifyMulticastListener,
//    which listens for incoming datagrams (serialized objects).
//    NotifyMulticastReceiver takes the datagram(s), reassembles them (if the
//    object was too big and therefore was fragmented) and pushed them in
//    the local EventChannel.
//
//
//  Changes:
//
//    1.1.0
//    - added subscription support
//
//    1.0.4
//    - added some debugging output
//
//    1.0.3
//    - changed namespace
//    - many clean ups
//    - renamed class to Receiver
//    - removed command/ping code and methods
//    - removed RtEC dependencies
//
//    1.0.2
//    - added ping facility
//    - added command facility
//
//    1.0.1.experimental
//    - added traffic analysis
//
//    1.0.0.experimental
//    - initial release, not tested!
//
//
//  Todo:
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef NotifyMulticastReceiver_h
#define NotifyMulticastReceiver_h

/* NotifyMulticast includes */
#include "NotifyMulticastDefines.h"
#include "NotifyMulticastRequestEntry.h"
#include "NotifyMulticastTimeoutHandler.h"
#include "NotifyMulticastParameters.h"

/* ACE includes */
#include <ace/SOCK_Dgram_Mcast.h>
#include <ace/Reactor.h>
#include <ace/Hash_Map_Manager.h>

/* TAO includes */
#include <orbsvcs/CosNotifyChannelAdminC.h>
#include <orbsvcs/CosNotifyCommC.h>

/* Miro includes */
#include <miro/StructuredPushSupplier.h>
#include <miro/Exception.h>

/* g++lib includes */
#include <vector>
#include <algorithm>

namespace Miro 
{
  namespace NMC 
  {
    // forward declerations
    class Adapter;
    class Config;
    class SH;

    class Receiver : public Miro::StructuredPushSupplier 
    {
      typedef Miro::StructuredPushSupplier Super;

    public:

      //------------------------------------------------------------------------
      // public types
      //------------------------------------------------------------------------

      typedef std::vector<unsigned long int> IPAddresses;

      //------------------------------------------------------------------------
      // public methods
      //------------------------------------------------------------------------

      //! Initializing constructor.
      Receiver(ACE_SOCK_Dgram_Mcast& _socket, 
	       CosNotifyChannelAdmin::EventChannel_ptr _ec,
	       std::string const& _domainName,
	       Parameters const * _params = Parameters::instance());

      //! Defaint destructor.
      ~Receiver();

      //! Called from EventHandler when input is available.
      int handle_input();

      //! Called from TimeoutHandler.
      int handle_timeout(ACE_Time_Value const& _tv, void const *_act);

      //! Sends an event through the MC-Notification-Channel.
      void sendEvent(CosNotification::StructuredEvent const & _event)
	throw(CosEventComm::Disconnected);

      //! Receive Data fram MC-Notification-Channel.
      int receiveData(iovec         *_iov,
		      ACE_INET_Addr &_from,
		      int            _flags = 0);

      // ACE_Time_Value timestamp(const std::string _name);

      void setSH(SH *_sh);
      void invalidateSH();

      friend class SH;

    protected:

      //------------------------------------------------------------------------
      // protected types
      //------------------------------------------------------------------------

      typedef ACE_Hash_Map_Manager<RequestIndex, RequestEntry*, ACE_SYNCH_NULL_MUTEX> RequestMap;
      typedef ACE_Hash_Map_Entry<RequestIndex, RequestEntry*> RequestMapEntry;

      struct EventData 
      {
	ACE_SOCK_Dgram_Mcast *socket;
	CORBA::Boolean        byteOrder;
	CORBA::ULong          requestId;
	CORBA::ULong          requestSize;
	CORBA::ULong          fragmentSize;
	CORBA::ULong          fragmentOffset;
	CORBA::ULong          fragmentId;
	CORBA::ULong          fragmentCount;
	CORBA::ULong          timestamp;
	CORBA::ULong          systemTimestamp;
      };

      //------------------------------------------------------------------------
      // protected methods
      //------------------------------------------------------------------------

      //! Check if address (_from) is local.
      bool is_loopback(ACE_INET_Addr const &_from);
      int handle_event(ACE_INET_Addr const& form,
		       EventData const& _eventData, 
		       char const *_iov,
		       unsigned long _len);

      //! Inherited interface method.
      /** StructuredUDPReceiver method */
      virtual void disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
	throw(CORBA::SystemException);

    private:
      //------------------------------------------------------------------------
      // private data
      //------------------------------------------------------------------------
      ACE_SOCK_Dgram_Mcast& socket_;
      Parameters const * params_;

      //! The map containing all the incoming requests which have been partially received.
      RequestMap            requestMap_;
      bool                  running_;
      int                   maxRetry_;
      Miro::Mutex           mutex_;

      unsigned long int     eventsReceived_;
      unsigned long int     eventsDropped_;
      unsigned long int     controlTrafficOut_;
      unsigned long int     controlTrafficIn_;
      unsigned long int     bytesTransmitted_;
      unsigned long int     maxAge_;
      unsigned char         dataId_;

      IPAddresses           localIPs_;

      unsigned int          droppedLocal_;

      bool shValid_;
      SH *sh_;
    };

    /**
     *  @param from: address to check
     */
    inline
    bool 
    Receiver::is_loopback(const ACE_INET_Addr &_from) 
    {
      return (std::binary_search(localIPs_.begin(), localIPs_.end(), _from.get_ip_address()));
    }
  }
}

#endif // NotifyMulticastReceiver_hh
