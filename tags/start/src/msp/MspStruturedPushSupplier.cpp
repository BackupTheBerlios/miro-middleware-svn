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


#include "MspStruturedPushSupplier.h"

namespace Msp
{
  StructuredPushSupplier::StructuredPushSupplier(CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin)
  {
    CosNotifyComm::StructuredPushSupplier_var objref = this->_this();
    CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
      supplier_admin->obtain_notification_push_consumer(CosNotifyChannelAdmin::STRUCTURED_EVENT, 
							proxy_consumer_id_);

    ACE_ASSERT(!CORBA::is_nil(proxyconsumer.in()));

    // narrow
    proxy_consumer_ =
      CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow(proxyconsumer.in());

    ACE_ASSERT(!CORBA::is_nil(proxy_consumer_.in()));

    proxy_consumer_->connect_structured_push_supplier(objref.in());
  }

  StructuredPushSupplier::~StructuredPushSupplier()
  {
    ACE_ASSERT(!CORBA::is_nil(proxy_consumer_.in()));

    proxy_consumer_->disconnect_structured_push_consumer();
  }

  void
  StructuredPushSupplier::subscription_change(const CosNotification::EventTypeSeq & /*added*/, 
                                              const CosNotification::EventTypeSeq & /*removed */
                                              ACE_ENV_ARG_DECL_NOT_USED)
    throw(CORBA::SystemException, CosNotifyComm::InvalidEventType)
  {
  }

  void
  StructuredPushSupplier::send_event(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  {
    ACE_ASSERT(!CORBA::is_nil(proxy_consumer_.in()));

    proxy_consumer_->push_structured_event(event);
  }

  void
  StructuredPushSupplier::disconnect_structured_push_supplier(CORBA::Environment &)
    throw(CORBA::SystemException)
  {
    // No-Op.
  }
};

