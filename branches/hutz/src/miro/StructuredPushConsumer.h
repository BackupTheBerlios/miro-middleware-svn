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
#ifndef StructuredPushConsumer_h
#define StructuredPushConsumer_h

#include "Synch.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushConsumer :
    public POA_CosNotifyComm::StructuredPushConsumer //,
//    public PortableServer::RefCountServantBase        
  {
  public:
    /** 
     *Initialization
     *
     * Connect the Consumer to the EventChannel.
     * Creates a new proxy supplier and connects to it.
     */
    StructuredPushConsumer(CosNotifyChannelAdmin::EventChannel_ptr _ec,
			   bool _connect = true);
    /** Disconnect from the supplier. */
    virtual ~StructuredPushConsumer();

    // these have to be non-virtual, since called
    // from the constructor
    void connect();
    void disconnect();

    /** Accessor for the Proxy that we're connected to. */
    CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr getProxySupplier();

    //  protected:
    /** The channel we connect to. */
    CosNotifyChannelAdmin::EventChannel_var ec_;
    /** The group operator between admin-proxy's. */
    CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;
    /** The id returned on creation of the consumer */
    CosNotifyChannelAdmin::AdminID consumerAdminId_;
    /** The consumer admin we use. */
    CosNotifyChannelAdmin::ConsumerAdmin_var consumerAdmin_;
    
  protected:
    /** The proxy that we are connected to. */
    CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxySupplier_;
    /** The proxy_supplier id. */
    CosNotifyChannelAdmin::ProxyID proxySupplierId_;

    Mutex connectedMutex_;
    bool connected_;

    // inherited IDL interface

    /** NotifyPublish method */
    virtual void offer_change(const CosNotification::EventTypeSeq & added,
                              const CosNotification::EventTypeSeq & removed
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

    /** StructuredPushSupplier methods */
    virtual void push_structured_event(const CosNotification::StructuredEvent & notification
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosEventComm::Disconnected);

    virtual void disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException);
  };
};
#endif
