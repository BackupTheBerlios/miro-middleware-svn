// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
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

  StructuredPushSupplier::~StructuredPushSupplier()
  {
    DBG(cout << "Destructing StructuredPushSupplier." << endl);

    disconnect();
  }

  void
  StructuredPushSupplier::connect() 
  {
    DBG(cout << "Connecting StructuredPushSupplier." << endl);

    Guard guard(connectedMutex_);
    if(!connected_) {
      StructuredPushSupplier_var objref = this->_this();

      supplierAdmin_ = ec_->new_for_suppliers(ifgop_, supplierAdminId_);

      ProxyConsumer_var proxyConsumer =
	supplierAdmin_->obtain_notification_push_consumer(CosNotifyChannelAdmin::STRUCTURED_EVENT, 
							  proxyConsumerId_);
      ACE_ASSERT(!CORBA::is_nil(proxyConsumer.in()));

      // narrow
      proxyConsumer_ =
	StructuredProxyPushConsumer::_narrow(proxyConsumer.in());
      ACE_ASSERT(!CORBA::is_nil(proxyConsumer.in()));

      proxyConsumer_->connect_structured_push_supplier(objref.in());
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

