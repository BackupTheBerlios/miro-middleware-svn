// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "StructuredPushSupplier.h"

#include <cstring>

#undef DEBUG
#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Miro
{
  using CosNotifyComm::StructuredPushSupplier_var;
  using CosNotifyChannelAdmin::EventChannel;
  using CosNotifyChannelAdmin::EventChannel_ptr;
  using CosNotifyChannelAdmin::SupplierAdmin_ptr;
  using CosNotifyChannelAdmin::ProxyConsumer_var;
  using CosNotifyChannelAdmin::StructuredProxyPushConsumer;
  
  /**
   * @param _ec Reference to the event channel.
   * @param _domainName The domain name of the event header.
   * This is equal to the robots naming context name
   * @param _connect Connect the Supplier to the EventChannel.
   * Creates a new proxy supplier and connects to it.
   */
  StructuredPushSupplier::StructuredPushSupplier(EventChannel_ptr _ec,
						 std::string const& _domainName,
						 bool _connect,
						 CosNotification::EventTypeSeq const& _event) :
    ec_(EventChannel::_duplicate(_ec)),
    ifgop_(CosNotifyChannelAdmin::OR_OP),
    supplierAdminId_(),
    supplierAdmin_(),
    domainName_(_domainName),
    connectedMutex_(),
    connected_(false),
    event_(_event),
    subscription_(event_.length(), false)
  {
    DBG(std::cout << "Constructing StructuredPushSupplier." << std::endl);

    if (_connect)
      connect();
  }

  /**
   * Disconnect the supplier from the proxy consumer.
   */
  StructuredPushSupplier::~StructuredPushSupplier()
  {
    DBG(std::cout << "Destructing StructuredPushSupplier." << std::endl);

    disconnect();
  }

  /**
   * As this method becomes called within the ctor, it can't be
   * virtual!
   */
  void
  StructuredPushSupplier::connect() 
  {
    DBG(std::cout << "Connecting StructuredPushSupplier." << std::endl);

    Guard guard(connectedMutex_);
    if(!connected_) {
      objref_ = this->_this();

      supplierAdmin_ = ec_->new_for_suppliers(ifgop_, supplierAdminId_);

      ProxyConsumer_var proxyConsumer =
	supplierAdmin_->obtain_notification_push_consumer(CosNotifyChannelAdmin::STRUCTURED_EVENT, 
							  proxyConsumerId_);
      ACE_ASSERT(!CORBA::is_nil(proxyConsumer.in()));

      // narrow
      proxyConsumer_ =
	StructuredProxyPushConsumer::_narrow(proxyConsumer.in());
      ACE_ASSERT(!CORBA::is_nil(proxyConsumer.in()));


      CosNotification::EventTypeSeq removed;
      removed.length(1);

      removed[0].domain_name =  CORBA::string_dup("*");
      removed[0].type_name = CORBA::string_dup("*");

      supplierAdmin_->offer_change(event_, removed);

      proxyConsumer_->connect_structured_push_supplier(objref_);
      connected_ = true;

      DBG(std::cout << "currently subscribed messages:" << std::endl);
      CosNotification::EventTypeSeq_var events = 
	proxyConsumer_->obtain_subscription_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);
      for (unsigned long i = 0; i < events->length(); ++i) {
	for (unsigned long j = 0; j < event_.length(); ++j) {
	  if (strcmp (events[i].type_name, event_[j].type_name) == 0 &&
	      strcmp (events[i].domain_name, event_[j].domain_name) == 0) {
	    subscription_[j] = true;
	    break;
	  }
	}
	DBG(std::cout << "  " << events[i].domain_name << "\t"
	    << "  " << events[i].type_name << std::endl);
      }
    }
  }

  void
  StructuredPushSupplier::disconnect() 
  {
    DBG(std::cout << "Disconnecting StructuredPushSupplier." << std::endl);

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

  const std::string& 
  StructuredPushSupplier::domainName()
  {
    return domainName_;
  }

  void
  StructuredPushSupplier::subscription_change(const CosNotification::EventTypeSeq& added,
					      const CosNotification::EventTypeSeq& removed
					      ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosNotifyComm::InvalidEventType)
  {
    DBG(std::cout << "subscription change" << std::endl;
	std::cout << "added messages:" << std::endl);
    for (unsigned long i = 0; i < added.length(); ++i) {
      for (unsigned long j = 0; j < event_.length(); ++j) {
	if (strcmp (added[i].type_name, event_[j].type_name) == 0 &&
	    strcmp (added[i].domain_name, event_[j].domain_name) == 0) {
	  subscription_[j] = true;
	  break;
	}
      }
      DBG(std::cout << "  " << added[i].domain_name << "\t"
	  << "  " << added[i].type_name << std::endl);
    }

    DBG(std::cout << "removed messages:" << std::endl);
    for (unsigned int i = 0; i < removed.length(); ++i) {
      for (unsigned long j = 0; j < event_.length(); ++j) {
	if (strcmp (removed[i].type_name, event_[j].type_name) == 0 &&
	    strcmp (removed[i].domain_name, event_[j].domain_name) == 0) {
	  subscription_[j] = false;
	  break;
	}
      }
      DBG(std::cout << "  " << removed[i].domain_name << "\t"
	  << "  " << removed[i].type_name << std::endl);
    }

#ifdef DEBUG
    std::cout << "currently subscribed messages:" << std::endl;
    CosNotification::EventTypeSeq_var events = 
      proxyConsumer_->obtain_subscription_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);
    for (unsigned int i=0; i < events->length(); ++i) {
      std::cout << "  " << events[i].domain_name << "\t"
	   << "  " << events[i].type_name << std::endl;
    }
#endif
  }

  void
  StructuredPushSupplier::disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException)
  {
    DBG(std::cout << "disconnect supplier" << std::endl);

    connected_ = false;
  }

  bool
  StructuredPushSupplier::subscribed(std::string const& _domain, 
				     std::string const& _type) const {
    for (unsigned long i = 0; i < event_.length(); ++i) {
      if (_domain == static_cast<char const *>(event_[i].domain_name) &&
	  _type == static_cast<char const *>(event_[i].type_name))
	return subscription_[i];
    }
    return false;
  }
}

