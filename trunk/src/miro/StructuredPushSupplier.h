// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// This code was originally taken from the orbsvs/examples/Notify/Supplier.
// AUTHOR is Pradeep Gore <pradeep@cs.wustl.edu>
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroSturcturedPushSupplier_h
#define miroSturcturedPushSupplier_h

#include "Synch.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include <string>
#include <vector>

namespace Miro
{
  class StructuredPushSupplier : public POA_CosNotifyComm::StructuredPushSupplier
  {
  public:
    //! Initializing constructor.
    StructuredPushSupplier(CosNotifyChannelAdmin::EventChannel_ptr  _ec,
			   const std::string& _domainName = std::string(),
			   bool _connect = true,
			   const CosNotification::EventTypeSeq & _event = CosNotification::EventTypeSeq());

    //! Destructor
    virtual ~StructuredPushSupplier();

    //! Connect to proxy consumer.
    void connect();
    //! Disconnect supplier to proxy consumer.
    void disconnect();

    bool subscribed(unsigned long _index) const;
    bool subscribed(std::string const& _domain, std::string const& _type) const;

    //! Send one event.
    void sendEvent(const CosNotification::StructuredEvent& event);
    
    //! Accessor method for the domain name.
    const std::string& domainName();

  protected:
    // inherited IDL interfae

    //! CosNotifyComm::StructuredPushSupplier interface method implementation.
    virtual void subscription_change (const CosNotification::EventTypeSeq & added,
				      const CosNotification::EventTypeSeq & removed
				      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

    //! CosNotifyComm::StructuredPushSupplier interface method implementation.
    virtual void disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException);

    typedef std::vector<bool> SubscriptionVector;

    //! The channel we connect to. 
    CosNotifyChannelAdmin::EventChannel_var ec_;
    //! The group operator between admin-proxy's. 
    CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;
    //! The suppllier admin id returned on supplier creation. 
    CosNotifyChannelAdmin::AdminID supplierAdminId_;
    //! The supplier admin used. 
    CosNotifyChannelAdmin::SupplierAdmin_var supplierAdmin_;

    //! The proxy that we are connected to. 
    CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxyConsumer_;
    //! This supplier's id. 
    CosNotifyChannelAdmin::ProxyID proxyConsumerId_;
    //! Our own object id.
    CosNotifyComm::StructuredPushSupplier_ptr objref_;

    //! Event domain name.
    std::string domainName_;

    //! Lock for the connected_ flag.
    Miro::Mutex connectedMutex_;
    //! If true, the supplier is connected to the event channel.
    bool connected_;

    CosNotification::EventTypeSeq event_;
    SubscriptionVector subscription_;
  };

  inline
  void
  StructuredPushSupplier::sendEvent(const CosNotification::StructuredEvent& event)
  {
//    Guard guard(connectedMutex_);
    if (connected_) {
      proxyConsumer_->push_structured_event(event);
    }
  }

  inline
  bool
  StructuredPushSupplier::subscribed(unsigned long _index) const {
    assert(_index < subscription_.size());
    return subscription_[_index];
  }
}
#endif
