// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "StructuredPushConsumer.h"

#undef DEBUG
#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_ptr;
using CosNotifyChannelAdmin::ConsumerAdmin_ptr;
using CosNotifyChannelAdmin::ProxySupplier_var;
using CosNotifyChannelAdmin::StructuredProxyPushSupplier;

namespace Miro
{
  StructuredPushConsumer::StructuredPushConsumer(EventChannel_ptr _ec,
						 bool _connect) :
    ec_(EventChannel::_duplicate(_ec)),
    ifgop_(CosNotifyChannelAdmin::OR_OP),
    consumerAdminId_(),
    consumerAdmin_(),
    connectedMutex_(),
    connected_(false)
  {
    DBG(cout << "Constructing StructuredPushConsumer." << endl);

    if (_connect)
      connect();
  }

  StructuredPushConsumer::~StructuredPushConsumer()
  {
    disconnect();
    DBG(cout << "Destructing StructuredPushConsumer." << endl);
  }

  void
  StructuredPushConsumer::connect()
  {
    DBG(cout << "Connecting StructuredPushConsumer." << endl);

    Guard guard(connectedMutex_);
    if (!connected_) {
      // Activate the consumer with the default_POA_
      CosNotifyComm::StructuredPushConsumer_var objref = this->_this();

      consumerAdmin_ = ec_->new_for_consumers(ifgop_, consumerAdminId_);
      ProxySupplier_var proxySupplier =
	consumerAdmin_->obtain_notification_push_supplier(CosNotifyChannelAdmin::STRUCTURED_EVENT, 
							  proxySupplierId_);
      assert(!CORBA::is_nil(proxySupplier.in()));

      // narrow
      proxySupplier_ = StructuredProxyPushSupplier::_narrow(proxySupplier.in());
      assert(!CORBA::is_nil (proxySupplier_.in()));

      proxySupplier_->connect_structured_push_consumer(objref.in());
      connected_ = true;

#ifdef DEBUG
      cout << "currently offered messages:" << endl;
      CosNotification::EventTypeSeq_var events = 
	proxySupplier_->obtain_offered_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);
      for (unsigned int i=0; i < events->length(); ++i) {
	cout << "  " << events[i].domain_name << "\t"
	     << "  " << events[i].type_name << endl;
      }
#endif
    }
  }

  void
  StructuredPushConsumer::disconnect()
  {
    Guard guard(connectedMutex_);
    if (connected_) {
      DBG(cout << "Disconnecting StructuredPushConsumer." << endl);
      connected_ = false;
      try {
	proxySupplier_->disconnect_structured_push_supplier();
	consumerAdmin_->destroy();
      }
      catch (const CORBA::Exception & e) {
	cerr << "StructuredPushConsumer::disconnect() CORBA exception on: " << endl
	     << e << endl;
      }
    }      
  }

  void
  StructuredPushConsumer::offer_change(const CosNotification::EventTypeSeq& DBG(added), 
                                       const CosNotification::EventTypeSeq& DBG(removed)
                                       ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosNotifyComm::InvalidEventType)
  {
#ifdef DEBUG
    cout << "StructuredPushConsumer: offer change" << endl;
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

    cout << "currently offered messages:" << endl;
    CosNotification::EventTypeSeq_var events = 
      proxySupplier_->obtain_offered_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);
    for (unsigned int i=0; i < events->length(); ++i) {
      cout << "  " << events[i].domain_name << "\t"
	   << "  " << events[i].type_name << endl;
    }
#endif
  }

  void
        StructuredPushConsumer::push_structured_event(const CosNotification::StructuredEvent & /*notification*/
                                                      ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosEventComm::Disconnected)
  {
    cerr << "You have to overwrite " 
	 << "StructuredPushConsumer::push_structured_event" << endl;
  }

  void
  StructuredPushConsumer::disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException)
  {
    DBG(cout << "StructuredPushConsumer: disconnect consumer" << endl);

    connected_ = false;
  }

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr
  StructuredPushConsumer::getProxySupplier()
  {
    return proxySupplier_.in();
  }
};

