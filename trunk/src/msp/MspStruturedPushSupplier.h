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
#ifndef mspSturcturedPushSupplier_hh
#define mspSturcturedPushSupplier_hh

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

namespace Msp
{

  class StructuredPushSupplier : public POA_CosNotifyComm::StructuredPushSupplier, 
				 public PortableServer::RefCountServantBase
  {
  public:
    /**
     * Connect the Supplier to the EventChannel.
     * Creates a new proxy supplier and connects to it.
     */
    StructuredPushSupplier(CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

    /**
     * Disconnect from the supplier.
     */
    virtual ~StructuredPushSupplier();

    /** Send one event. */
    virtual void send_event(const CosNotification::StructuredEvent& event);
    
  protected:
    /** The proxy that we are connected to. */
    CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;
    /** This supplier's id. */
    CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;

    /** Destructor */

    /** NotifySubscribe */
    virtual void subscription_change (const CosNotification::EventTypeSeq & added,
				      const CosNotification::EventTypeSeq & removed
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

    /** StructuredPushSupplier method */
    virtual void disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      throw(CORBA::SystemException);
  };
};
#endif
