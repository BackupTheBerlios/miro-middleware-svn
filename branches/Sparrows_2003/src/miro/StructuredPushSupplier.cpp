// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "StructuredPushSupplier.h"

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
						 const std::string& _domainName,
						 bool _connect) :
    ec_(EventChannel::_duplicate(_ec)),
    ifgop_(CosNotifyChannelAdmin::OR_OP),
    supplierAdminId_(),
    supplierAdmin_(),
    domainName_(_domainName),
    connectedMutex_(),
    connected_(false)
  {
    DBG(cout << "Constructing StructuredPushSupplier." << endl);

    if (_connect)
      connect();
  }

  /**
   * Disconnect the supplier from the proxy consumer.
   */
  StructuredPushSupplier::~StructuredPushSupplier()
  {
    DBG(cout << "Destructing StructuredPushSupplier." << endl);

    disconnect();
  }

  /**
   * As this method becomes called within the ctor, it can't be
   * virtual!
   */
  void
  StructuredPushSupplier::connect() 
  {
    DBG(cout << "Connecting StructuredPushSupplier." << endl);

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

      proxyConsumer_->connect_structured_push_supplier(objref_);
      connected_ = true;

#ifdef DEBUG
      cout << "currently subscribed messages:" << endl;
      CosNotification::EventTypeSeq_var events = 
	proxyConsumer_->obtain_subscription_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);
      for (unsigned int i=0; i < events->length(); ++i) {
	cout << "  " << events[i].domain_name << "\t"
	     << "  " << events[i].type_name << endl;
      }
#endif
    }
  }
  void
  StructuredPushSupplier::disconnect() 
  {
    DBG(cout << "Disconnecting StructuredPushSupplier." << endl);

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
  StructuredPushSupplier::subscription_change(const CosNotification::EventTypeSeq& DBG(added),
					      const CosNotification::EventTypeSeq& DBG(removed)
					      ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosNotifyComm::InvalidEventType)
  {
#ifdef DEBUG
    cout << "subscription change" << endl;
    cout << "added messages:" << endl;
    for (unsigned int i=0; i < added.length(); ++i) {
      cout << "  " << added[i].domain_name << "\t"
	   << "  " << added[i].type_name << endl;
    }
    cout << "removed messages:" << endl;
    for (unsigned int i=0; i < removed.length(); ++i) {
      cout << "  " << removed[i].domain_name << "\t"
	   << "  " << removed[i].type_name << endl;
    }

    cout << "currently subscribed messages:" << endl;
    CosNotification::EventTypeSeq_var events = 
      proxyConsumer_->obtain_subscription_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);
    for (unsigned int i=0; i < events->length(); ++i) {
      cout << "  " << events[i].domain_name << "\t"
	   << "  " << events[i].type_name << endl;
    }

#endif
  }

  void
  StructuredPushSupplier::disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException)
  {
    DBG(cout << "disconnect supplier" << endl);

    connected_ = false;
  }
};

