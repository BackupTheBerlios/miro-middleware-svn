// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "StructuredPushConsumer.h"

#include <cstring>
#include <miro/Log.h>

namespace Miro
{
  using ::operator<<;

  /**
   * @param _ec Reference to the event channel.
   * @param _connect Connect the consumer to the event channel.
   * @param _subscriptions Initial list of subscriptions.
   */
  StructuredPushConsumer::StructuredPushConsumer(CosNotifyChannelAdmin::EventChannel_ptr _ec,
						 bool _connect,
						 CosNotification::EventTypeSeq  const& _subscriptions) :
    ec_(CosNotifyChannelAdmin::EventChannel::_duplicate(_ec)),
    ifgop_(CosNotifyChannelAdmin::OR_OP),
    consumerAdminId_(),
    consumerAdmin_(),
    connectedMutex_(),
    connected_(0),
    subscribed_(false),
    subscriptions_(asterixSubscription()),
    offers_(subscriptions_.length(), false)
  {
    MIRO_DBG(MIRO,LL_CTOR_DTOR, "Constructing StructuredPushConsumer.\n");

    consumerAdmin_ = ec_->new_for_consumers(ifgop_, consumerAdminId_);
    CosNotifyChannelAdmin::ProxySupplier_var proxySupplier = 
      consumerAdmin_->
      obtain_notification_push_supplier(CosNotifyChannelAdmin::STRUCTURED_EVENT, 
					proxySupplierId_);
    MIRO_ASSERT(!CORBA::is_nil(proxySupplier.in()));
    
    // narrow
    proxySupplier_ = 
      CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(proxySupplier.in());
    MIRO_ASSERT(!CORBA::is_nil (proxySupplier_.in()));
    
    setSubscriptions(_subscriptions);

    if (_connect)
      connect();
  }

  /** Disconnect from the event channel. */
  StructuredPushConsumer::~StructuredPushConsumer()
  {
    disconnect();
    consumerAdmin_->destroy();
    MIRO_DBG(MIRO,LL_CTOR_DTOR, "Destructing StructuredPushConsumer.\n");
  }

  void
  StructuredPushConsumer::connect()
  {
    MIRO_DBG(MIRO,LL_NOTICE,"Connecting StructuredPushConsumer.\n");

    Guard guard(connectedMutex_);

    if (connected_ == 0) {
      // Activate the consumer with the default_POA_
      objref_ = this->_this();

      proxySupplier_->connect_structured_push_consumer(objref_);
      connected_ = 1;

      MIRO_DBG(MIRO,LL_PRATTLE, "currently offered messages:\n");

      CosNotification::EventTypeSeq_var events = 
	proxySupplier_->obtain_offered_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);
      for (unsigned long i = 0; i < events->length(); ++i) {
	for (unsigned long j = 0; j < subscriptions_.length(); ++j) {
	  if (strcmp (events[i].type_name, subscriptions_[j].type_name) == 0 &&
	      strcmp (events[i].domain_name, subscriptions_[j].domain_name) == 0) {
	    offers_[j] = true;
	    break;
	  }
	}
	MIRO_DBG_OSTR(MIRO,LL_PRATTLE, "  " << events[i].domain_name << "\t"
	    << "  " << events[i].type_name << std::endl);
      }
    }
  }

  void
  StructuredPushConsumer::disconnect()
  {
    Guard guard(connectedMutex_);
    if (connected_ == 1) {
      MIRO_DBG(MIRO,LL_NOTICE,"Disconnecting StructuredPushConsumer.\n");
      connected_ = -1;
      try {
	proxySupplier_->disconnect_structured_push_supplier();
	  
	// Get reference to Root POA.
	PortableServer::POA_var poa = _default_POA();

	// Deactivate.
	PortableServer::ObjectId_var oid =
	  poa->reference_to_id (objref_);

	CORBA::release(objref_);
    
	// deactivate from the poa.
	poa->deactivate_object (oid.in ());
      }
      catch (const CORBA::Exception & e) {
         MIRO_LOG_OSTR(LL_ERROR,"StructuredPushConsumer::disconnect() CORBA exception on: " << std::endl
	     << e << std::endl);
      }
    }      
  }

  /**
   * @param domain Event domain name.
   * @param type Event type name.
   */
  bool
  StructuredPushConsumer::offered(std::string const& _domain, 
				  std::string const& _type) const 
  {
    Guard guard(connectedMutex_);

    for (unsigned long i = 0; i < subscriptions_.length(); ++i) {
      if (_domain == static_cast<char const *>(subscriptions_[i].domain_name) &&
	  _type == static_cast<char const *>(subscriptions_[i].type_name))
	return offers_[i];
    }
    return false;
  }

  void
  StructuredPushConsumer::subscribe()
  {
    subscribed_ = true;
  }

  void
  StructuredPushConsumer::unsubscribe()
  {
    subscribed_ = false;
  }

  /** 
   * The consumer will register the subscriptions at its event consumer admin. 
   * The admin will make sure, the notification channel suppliers are informed
   * about new subscription. That way, suppliers can query whether the events they
   * offer are currently subscribed by any supplier.
   *
   * Internally the consumer keeps track of the events offered by suppliers.
   * A user of the consumer can query at the consumer for each subscribed event,
   * wheter it is offered by any supplier or not. This way an event sink
   * can determine whether the demanded information is produced by the system.
   *
   * For efficient querying of offer/subscription matches, this method returns a
   * vector containing a handle for each added subscription. This handle can be used
   * for efficient querying of the availability of suppliers for a 
   * particular event type vie the offered method.
   */
  StructuredPushConsumer::IndexVector
  StructuredPushConsumer::addSubscriptons(CosNotification::EventTypeSeq const& _added)
  {
    Guard guard(connectedMutex_);

    CORBA::ULong subscriptionsLen = subscriptions_.length();
    CORBA::ULong addedLen = _added.length();
    CORBA::ULong newSubscriptionsLen = subscriptionsLen;

    // assigned indexes 
    IndexVector indexes(addedLen);

    // actually added events
    CosNotification::EventTypeSeq added;
    // dummy: allway empty
    CosNotification::EventTypeSeq removed;

    // enlarge the offers vector by the maximum required size
    subscriptions_.length(subscriptionsLen + addedLen);
    added.length(addedLen);

    for (unsigned int i = 0; i < addedLen; ++i) {
      unsigned int j = 0;
      for (; j < subscriptionsLen; ++j) {
	// search whether already offered
	if (strcmp(subscriptions_[j].type_name, _added[i].type_name) == 0 &&
	    strcmp(subscriptions_[j].domain_name, _added[i].domain_name) == 0) {
	  indexes[i] = j;
	  break;
	}
      }

      // add new offer to list of offers
      if (j == subscriptionsLen) {
	subscriptions_[newSubscriptionsLen] = _added[i];
	added[newSubscriptionsLen - subscriptionsLen] = _added[i];
	indexes[i] = newSubscriptionsLen;
	++newSubscriptionsLen;
      }
    }
    // resize offers vector to actual size
    subscriptions_.length(newSubscriptionsLen);
    added.length(newSubscriptionsLen - subscriptionsLen);

    initiateSubscriptionChange(added, removed);

    return indexes;
  }

  /**
   *
   * This will overwrite all previously set or added subscriptions.
   *
   * The consumer will register the subscriptions at its event consumer admin. 
   * The admin will make sure, the notification channel suppliers are informed
   * about new subscriptions. That way, suppliers can query whether events it
   * offers are currently subscribed by any consumer.
   *
   * Internally the consumer keeps track of the events offered by suppliers.
   * A user of the consumer can query at the consumer for each subscribed event,
   * whether it is offered by any supplier or not. This way an event sink
   * can determine whether the demanded information is produced by the system.
   *
   * For efficiency, offer/subscription matches can be queried by the index,
   * they apeared within the orgument vector of this method.
   *
   * @param offers The vector of new subscirptions.
   */
  void 
  StructuredPushConsumer::setSubscriptions(CosNotification::EventTypeSeq const& _newSubscriptions,
					   bool _subscribe)
  {
    Guard guard(connectedMutex_);

    CORBA::ULong offersLen = subscriptions_.length();
    CORBA::ULong newOffersLen = _newSubscriptions.length();

    // actually added events
    CosNotification::EventTypeSeq added;
    // dummy: always empty
    CosNotification::EventTypeSeq removed;

    CORBA::ULong addedIndex = 0;
    CORBA::ULong removedIndex = 0;

    // enlarge the offers vector by the maximum required size
    added.length(newOffersLen);
    removed.length(offersLen);

    for (unsigned int i = 0; i < newOffersLen; ++i) {
      unsigned int j;
      for (j = 0; j < offersLen; ++j) {
	// search whether already offered
	if (strcmp(_newSubscriptions[i].type_name, subscriptions_[j].type_name) == 0 &&
	    strcmp(_newSubscriptions[i].domain_name, subscriptions_[j].domain_name) == 0) {
	  break;
	}
      }

      // add new offer to list of added offers
      if (j == offersLen) {
	added[addedIndex] = _newSubscriptions[i];
	++addedIndex;
      }
    }

    for (unsigned int i = 0; i < offersLen; ++i) {
      unsigned int j;
      for (j = 0; j < newOffersLen; ++j) {
	// search whether still offered
	if (strcmp(subscriptions_[i].type_name, _newSubscriptions[j].type_name) == 0 &&
	    strcmp(subscriptions_[i].domain_name, _newSubscriptions[j].domain_name) == 0) {
	  break;
	}
      }

      // add odl offer to list of removed offers
      if (j == newOffersLen) {
	removed[removedIndex] = subscriptions_[i];
	++removedIndex;
      }
    }

    // resize offers vector to actual size
    added.length(addedIndex);
    removed.length(removedIndex);

    // overwrite offers vector
    subscriptions_ = _newSubscriptions;

    // do the offers change and performe bookkeeping
    initiateSubscriptionChange(added, removed);

    subscribed_ = _subscribe;
  }

  void
  StructuredPushConsumer::offer_change(const CosNotification::EventTypeSeq& added, 
                                       const CosNotification::EventTypeSeq& removed
                                       ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosNotifyComm::InvalidEventType)
  {
    MIRO_DBG_OSTR(MIRO, LL_PRATTLE, "StructuredPushConsumer: offer change" << std::endl
	<< "subscription change" << std::endl
	<< "added messages:" << std::endl);
    for (unsigned long i = 0; i < added.length(); ++i) {
      for (unsigned long j = 0; j < subscriptions_.length(); ++j) {
	if (strcmp (added[i].type_name, subscriptions_[j].type_name) == 0 &&
	    strcmp (added[i].domain_name, subscriptions_[j].domain_name) == 0) {
	  offers_[j] = true;
	  break;
	}
      }
      MIRO_DBG_OSTR(MIRO,LL_PRATTLE," " << added[i].domain_name << "\t"
	  << "  " << added[i].type_name << std::endl);
    }
    MIRO_DBG(MIRO,LL_PRATTLE,"removed messages:\n");
    for (unsigned long i = 0; i < removed.length(); ++i) {
      for (unsigned long j = 0; j < subscriptions_.length(); ++j) {
	if (strcmp (removed[i].type_name, subscriptions_[j].type_name) == 0 &&
	    strcmp (removed[i].domain_name, subscriptions_[j].domain_name) == 0) {
	  offers_[j] = false;
	  break;
	}
      }
      MIRO_DBG_OSTR(MIRO,LL_PRATTLE, "  " << removed[i].domain_name << "\t"
	  << "  " << removed[i].type_name << std::endl);
    }
  }

  void
  StructuredPushConsumer::push_structured_event(const CosNotification::StructuredEvent & /*notification*/
						ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosEventComm::Disconnected)
  {
     MIRO_LOG(LL_ERROR, "You have to overwrite StructuredPushConsumer::push_structured_event\n");
  }

  void
  StructuredPushConsumer::disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException)
  {
    MIRO_DBG(MIRO,LL_NOTICE,"StructuredPushConsumer: disconnect consumer\n");

    connected_ = -1;
  }

  void
  StructuredPushConsumer::initiateSubscriptionChange(CosNotification::EventTypeSeq const& _added,
						     CosNotification::EventTypeSeq const& _removed)
  {
    // reserve space in subscriptions vector
    offers_.resize(subscriptions_.length(), false);

    // inform the supplier admin about the new offers
    consumerAdmin_->subscription_change(_added, _removed);
      
    // generate list of subscribed offers
    CosNotification::EventTypeSeq_var offers = 
      proxySupplier_->obtain_offered_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);
    for (unsigned long i = 0; i < offers->length(); ++i) {
      for (unsigned long j = 0; j < subscriptions_.length(); ++j) {
	if (strcmp(offers[i].type_name, subscriptions_[j].type_name) == 0 &&
	    strcmp(offers[i].domain_name, subscriptions_[j].domain_name) == 0) {
	  offers_[i] = true;
	  break;
	}
      }
    }
  }

  CosNotification::EventTypeSeq
  StructuredPushConsumer::asterixSubscription() 
  {
    CosNotification::EventTypeSeq offer;

    offer.length(1);
    offer[0].type_name = CORBA::string_dup("*");
    offer[0].domain_name = CORBA::string_dup("*");

    return offer;
  }
}

