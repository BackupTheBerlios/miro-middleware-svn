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

#include "StructuredPushSupplier.h"

#include <cstring>
#include "Log.h"

#undef DEBUG

namespace Miro
{
  /**
   * Create a new proxy supplier.
   *
   * @param _ec Reference to the event channel.
   * @param _domainName The domain name of the event header.
   * This is equal to the robots naming context name
   * @param _connect Connect the supplier to the event channel.
   * @param _offers Initial set of offers.
   */
  StructuredPushSupplier::StructuredPushSupplier(CosNotifyChannelAdmin::EventChannel_ptr _ec,
						 std::string const& _domainName,
						 bool _connect,
						 CosNotification::EventTypeSeq const& _offers) :
    ec_(CosNotifyChannelAdmin::EventChannel::_duplicate(_ec)),
    ifgop_(CosNotifyChannelAdmin::OR_OP),
    supplierAdminId_(),
    supplierAdmin_(),
    domainName_(_domainName),
    connectedMutex_(),
    connected_(false),
    offers_(asterixOffer()),
    subscription_(1, false)
  {
    MIRO_DBG(MIRO,LL_CTOR_DTOR,"Constructing StructuredPushSupplier.\n");

    // Obtain new admin.
    // As domain/type based filtering is admin based, we create one
    // admin per supplier.
    supplierAdmin_ = ec_->new_for_suppliers(ifgop_, supplierAdminId_);

    // Init proxy consumer
     CosNotifyChannelAdmin::ProxyConsumer_var proxyConsumer =
      supplierAdmin_->
       obtain_notification_push_consumer(CosNotifyChannelAdmin::STRUCTURED_EVENT, 
					 proxyConsumerId_);
    MIRO_ASSERT(!CORBA::is_nil(proxyConsumer.in()));

    // narrow
    proxyConsumer_ =
      CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow(proxyConsumer.in());
    MIRO_ASSERT(!CORBA::is_nil(proxyConsumer.in()));

    // set the offers list before connecting.
    setOffers(_offers);

    if (_connect)
      connect();
  }

  /** Disconnect from the event channel. */
  StructuredPushSupplier::~StructuredPushSupplier()
  {
    MIRO_DBG(MIRO,LL_CTOR_DTOR, "Destructing StructuredPushSupplier.\n");

    disconnect();
  }

  void
  StructuredPushSupplier::connect() 
  {
    MIRO_DBG(MIRO,LL_PRATTLE, "Connecting StructuredPushSupplier.\n");

    Guard guard(connectedMutex_);
    if (!connected_) {
      // register object at the POA
      objref_ = this->_this();
      // connect to the proxy consumer.
      proxyConsumer_->connect_structured_push_supplier(objref_);
      connected_ = true;
    }
  }

  void
  StructuredPushSupplier::disconnect() 
  {
    MIRO_DBG(MIRO,LL_PRATTLE,"Disconnecting StructuredPushSupplier.\n");

    Guard guard(connectedMutex_);
    if (connected_) {
      proxyConsumer_->disconnect_structured_push_consumer();
      supplierAdmin_->destroy();

      // Get reference to Root POA.
      PortableServer::POA_var poa = _default_POA();

      // Deactivate.
      PortableServer::ObjectId_var oid =
	poa->reference_to_id (objref_);

      CORBA::release(objref_);
    
      // deactivate from the poa.
      poa->deactivate_object (oid.in ());

      connected_ = false;
    }
  }

  /**
   * @param domain Event domain name.
   * @param type Event type name.
   */
  bool
  StructuredPushSupplier::subscribed(std::string const& _domain, 
				     std::string const& _type) const
  {
    Guard guard(connectedMutex_);
    CORBA::ULong offersLen = offers_.length();

    for (unsigned long i = 0; i < offersLen; ++i) {
      if (_domain == static_cast<char const *>(offers_[i].domain_name) &&
	  _type == static_cast<char const *>(offers_[i].type_name))
	return subscription_[i];
    }
    return false;
  }

  /** 
   * The supplier will register the offers at its event supplier admin. 
   * The admin will make sure, the notification channel consumers are informed
   * about new offers. That way, consumers can query whether the events it
   * subscribes is currently offered by any supplier.
   *
   * Internally the supplier keeps track of the events subscirbed by consumers.
   * A user of the supplier can query at the supplier for each offered event,
   * whether it is subscribed by any consumer or not. This way an event source
   * can skip the production of any event that no consumer currently is 
   * subscribed for.
   *
   * @param added The vector of added offers.
   *
   * @return For efficient querying of subscription/offer matches,
   * this method returns a vector containing a handle for each added
   * offer. This handle can be used for efficient querying of the
   * availability of consumers for a particular event type vie the
   * subscribed method.
   */
  StructuredPushSupplier::IndexVector
  StructuredPushSupplier::addOffers(CosNotification::EventTypeSeq const& _added)
  {
    Guard guard(connectedMutex_);

    CORBA::ULong offersLen = offers_.length();
    CORBA::ULong addedLen = _added.length();
    CORBA::ULong newOffersLen = offersLen;

    // assigned indexes 
    IndexVector indexes(addedLen);

    // actually added events
    CosNotification::EventTypeSeq added;
    // dummy: allway empty
    CosNotification::EventTypeSeq removed;

    // enlarge the offers vector by the maximum required size
    offers_.length(offersLen + addedLen);
    added.length(addedLen);

    for (unsigned int i = 0; i < addedLen; ++i) {
      unsigned int j = 0;
      for (; j < offersLen; ++j) {
	// search whether already offered
	if (strcmp(offers_[j].type_name, _added[i].type_name) == 0 &&
	    strcmp(offers_[j].domain_name, _added[i].domain_name) == 0) {
	  indexes[i] = j;
	  break;
	}
      }

      // add new offer to list of offers
      if (j == offersLen) {
	offers_[newOffersLen] = _added[i];
	added[newOffersLen - offersLen] = _added[i];
	indexes[i] = newOffersLen;
	++newOffersLen;
      }
    }
    // resize offers vector to actual size
    offers_.length(newOffersLen);
    added.length(newOffersLen - offersLen);

    // do the offers change and performe bookkeeping
    initiateOfferChange(added, removed);

    return indexes;
  }

  /**
   *
   * This will overwrite all previously set or added offers.
   *
   * The supplier will register the offers at its event supplier admin. 
   * The admin will make sure, the notification channel consumers are informed
   * about new offers. That way, consumers can query whether the events it
   * subscribes is currently offered by any supplier.
   *
   * Internally the supplier keeps track of the events subscirbed by consumers.
   * A user of the supplier can query at the supplier for each offered event,
   * whether it is subscribed by any consumer or not. This way an event source
   * can skip the production of any event that no consumer currently is 
   * subscribed for.
   *
   * For efficiency, subscription/offer matches can be queried by the index,
   * they apeared within the orgument vector of this method.
   *
   * @param offers The vector of new offers.
   */
  void
  StructuredPushSupplier::setOffers(CosNotification::EventTypeSeq const& _newOffers)
  {
    Guard guard(connectedMutex_);

    CORBA::ULong offersLen = offers_.length();
    CORBA::ULong newOffersLen = _newOffers.length();

    // actually added events
    CosNotification::EventTypeSeq added;
    // dummy: allway empty
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
	if (strcmp(_newOffers[i].type_name, offers_[j].type_name) == 0 &&
	    strcmp(_newOffers[i].domain_name, offers_[j].domain_name) == 0) {
	  break;
	}
      }

      // add new offer to list of offers
      if (j == offersLen) {
	added[addedIndex] = _newOffers[i];
	++addedIndex;
      }
    }

    for (unsigned int i = 0; i < offersLen; ++i) {
      unsigned int j;
      for (j = 0; j < newOffersLen; ++j) {
	// search whether already offered
	if (strcmp(offers_[i].type_name, _newOffers[j].type_name) == 0 &&
	    strcmp(offers_[i].domain_name, _newOffers[j].domain_name) == 0) {
	  break;
	}
      }

      // add new offer to list of offers
      if (j == newOffersLen) {
	removed[removedIndex] = offers_[i];
	++removedIndex;
      }
    }

    // resize offers vector to actual size
    added.length(addedIndex);
    removed.length(removedIndex);

    // overwrite offers vector
    offers_ = _newOffers;

    // do the offers change and performe bookkeeping
    initiateOfferChange(added, removed);
  }

  void
  StructuredPushSupplier::subscription_change(CosNotification::EventTypeSeq const& added,
					      CosNotification::EventTypeSeq const& removed
					      ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosNotifyComm::InvalidEventType)
  {
    MIRO_DBG(MIRO,LL_PRATTLE,"subscription change\nadded messages:\n");

    for (unsigned long i = 0; i < added.length(); ++i) {
      for (unsigned long j = 0; j < offers_.length(); ++j) {
	if (strcmp (added[i].type_name, offers_[j].type_name) == 0 &&
	    strcmp (added[i].domain_name, offers_[j].domain_name) == 0) {
	  subscription_[j] = true;
	  break;
	}
      }
      MIRO_DBG_OSTR(MIRO,LL_PRATTLE, "  " << added[i].domain_name << "\t"
	  << "  " << added[i].type_name << std::endl);
    }

    MIRO_DBG(MIRO,LL_PRATTLE,"removed messages:\n");
    for (unsigned int i = 0; i < removed.length(); ++i) {
      for (unsigned long j = 0; j < offers_.length(); ++j) {
	if (strcmp (removed[i].type_name, offers_[j].type_name) == 0 &&
	    strcmp (removed[i].domain_name, offers_[j].domain_name) == 0) {
	  subscription_[j] = false;
	  break;
	}
      }
      MIRO_DBG_OSTR(MIRO,LL_PRATTLE,"  " << removed[i].domain_name << "\t"
	  << "  " << removed[i].type_name << std::endl);
    }
  }

  void
  StructuredPushSupplier::disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException)
  {
    MIRO_DBG(MIRO,LL_PRATTLE, "disconnect supplier\n");

    connected_ = false;
  }

  void
  StructuredPushSupplier::initiateOfferChange(CosNotification::EventTypeSeq const& _added,
					      CosNotification::EventTypeSeq const& _removed)
  {
    // reserve space in subscriptions vector
    subscription_.resize(offers_.length(), false);

    // inform the supplier admin about the new offers
    supplierAdmin_->offer_change(_added, _removed);
      
    // generate list of subscribed offers
    CosNotification::EventTypeSeq_var subscritpions = 
      proxyConsumer_->obtain_subscription_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);
    for (unsigned long i = 0; i < offers_.length(); ++i) {
      for (unsigned long j = 0; j < subscritpions->length(); ++j) {
	if (strcmp(offers_[i].type_name, subscritpions[j].type_name) == 0 &&
	    strcmp(offers_[i].domain_name, subscritpions[j].domain_name) == 0) {
	  subscription_[i] = true;
	  break;
	}
      }
    }
  }

  CosNotification::EventTypeSeq
  StructuredPushSupplier::asterixOffer() 
  {
    CosNotification::EventTypeSeq offer;

    offer.length(1);
    offer[0].type_name = CORBA::string_dup("*");
    offer[0].domain_name = CORBA::string_dup("*");

    return offer;
  }
}

