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
// This code is taken from the orbsvs/examples/Notify/Supplier.
// AUTHOR is Pradeep Gore <pradeep@cs.wustl.edu>
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroSturcturedPushSupplier_hh
#define miroSturcturedPushSupplier_hh

#include "Synch.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include <string>

namespace Miro
{
  class StructuredPushSupplier : public POA_CosNotifyComm::StructuredPushSupplier
  {
  public:
    /**
     * Connect the Supplier to the EventChannel.
     * Creates a new proxy supplier and connects to it.
     */
    StructuredPushSupplier(CosNotifyChannelAdmin::EventChannel_ptr  _ec,
			   const std::string                       &_domainName = std::string(),
			   bool                                     _connect = true);

    /**
     * Disconnect from the supplier.
     */
    virtual ~StructuredPushSupplier();

    // these can't be virtual!
    void connect();
    void disconnect();

    /** Send one event. */
    void sendEvent(const CosNotification::StructuredEvent& event);
    
    const std::string& domainName();

  protected:
    /** The channel we connect to. */
    CosNotifyChannelAdmin::EventChannel_var ec_;
    /** The group operator between admin-proxy's. */
    CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;
    /** The suppllier admin id returned on supplier creation. */
    CosNotifyChannelAdmin::AdminID supplierAdminId_;
    /** The supplier admin used. */
    CosNotifyChannelAdmin::SupplierAdmin_var supplierAdmin_;

    /** The proxy that we are connected to. */
    CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxyConsumer_;
    /** This supplier's id. */
    CosNotifyChannelAdmin::ProxyID proxyConsumerId_;

    /** message domain name */
    std::string domainName_;

    Miro::Mutex connectedMutex_;
    bool connected_;

    // inherited IDL interfae

    /** NotifySubscribe */
    virtual void subscription_change (const CosNotification::EventTypeSeq & added,
				      const CosNotification::EventTypeSeq & removed
				      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

    /** StructuredPushSupplier method */
    virtual void disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException);
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
};
#endif
