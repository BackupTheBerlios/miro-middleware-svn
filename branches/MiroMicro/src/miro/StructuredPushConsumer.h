// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#ifndef StructuredPushConsumer_h
#define StructuredPushConsumer_h

#include "Synch.h"
#include "Log.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include <vector>
#include <string>

namespace Miro
{
  //! StructuredPushConsumerr interface implementation.
  /**
   * This class implements the StructuredPushConsumer interface of the
   * CORBA notification service. It performs bookkeeping of offers
   * from suplliers to the events subscribed by this consumer and
   * allowes efficient querying of this information for event
   * consumers.
   */
  class StructuredPushConsumer :
        public POA_CosNotifyComm::StructuredPushConsumer
  {
  public:
    //--------------------------------------------------------------------------
    // public types
    //--------------------------------------------------------------------------

    typedef std::vector<unsigned long> IndexVector;

    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    //! Initializing constructor.
    StructuredPushConsumer(CosNotifyChannelAdmin::EventChannel_ptr _ec,
                           bool _connect = true,
                           const CosNotification::EventTypeSeq & _subscription =
                             CosNotification::EventTypeSeq());

    //! Destructor
    virtual ~StructuredPushConsumer();

    //! Connect to proxy supplier.
    void connect();
    //! Disconnect from proxy supplier.
    void disconnect();
    //! Report the connection status.
    bool connected() const;

    //! Subscribe the subscriptions from the notifcation channel.
    /**
     * This is for deferred subscription after
     * @ref setSubscriptions() or addSubscriptons() calls or
     * resubscripiton after @ref unsubscribe() calls.
     */
    void subscribe();
    //! Unsubscribe the subscriptions from the notifcation channel.
    /**
     * Temporarily unsubscribe the set messages.
     */
    void unsubscribe();
    //! Report the subscription status.
    bool subscribed() const;

    //! Test whether an subscribed event is offered.
    bool offered(unsigned long _index) const;
    //! Test whether an subscribed event is offered.
    bool offered(std::string const& _domain, std::string const& _type) const;

    //! Add a set of subscriptions from the notification channel.
    IndexVector addSubscriptons(CosNotification::EventTypeSeq const& _added);
    //! Set the set of subscriptions from the notification channel.
    void setSubscriptions(CosNotification::EventTypeSeq const& _newSubscriptions,
                          bool subscribe = true);
    //! Set a single subscription from the notification channel.
    /** This is a convenience method, that just calls setSubscriptions.
     * The subscription will have the index 0, for further reference. */
    void setSingleSubscription(std::string const& _domain, std::string const& _type);

    //! Accessor for the Proxy that we're connected to.
    CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr getProxySupplier();

  protected:
    //--------------------------------------------------------------------------
    // protected type definitions
    //--------------------------------------------------------------------------

    typedef std::vector<bool> OfferVector;

    //--------------------------------------------------------------------------
    // protected methods
    //--------------------------------------------------------------------------

    //! Tell the admin about an subscription change and update the offer vector.
    void initiateSubscriptionChange(CosNotification::EventTypeSeq const& _added,
                                    CosNotification::EventTypeSeq const& _removed);
    // inherited IDL interface

    //! Callback for the admin to inform about changes from the supplier side.
    virtual void offer_change(const CosNotification::EventTypeSeq & added,
                              const CosNotification::EventTypeSeq & removed
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

    //! Callback for the admin to push events to the client.
    virtual void push_structured_event(const CosNotification::StructuredEvent & notification
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    throw(CORBA::SystemException, CosEventComm::Disconnected);

    //! Callback for the admin to tell the consumer about a disconnect.
    virtual void disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    throw(CORBA::SystemException);

    //--------------------------------------------------------------------------
    // protected static methods
    //--------------------------------------------------------------------------

    //! Helper method to initialize the default subscription.
    static CosNotification::EventTypeSeq asterixSubscription();

    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------

    //! The channel we connect to.
    CosNotifyChannelAdmin::EventChannel_var ec_;
    //! The group operator between admin-proxy's.
    CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;
    //! The id returned on creation of the consumer
    CosNotifyChannelAdmin::AdminID consumerAdminId_;

  public:
    //! The consumer admin we use.
    CosNotifyChannelAdmin::ConsumerAdmin_var consumerAdmin_;

  protected:
    //! The proxy that we are connected to.
    CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxySupplier_;
    //! The proxy_supplier id.
    CosNotifyChannelAdmin::ProxyID proxySupplierId_;
    //! Our own object id.
    CosNotifyComm::StructuredPushConsumer_ptr objref_;

    //! Lock for the connected_ flag.
    mutable Mutex connectedMutex_;
    //! If true, the supplier is connected to the event channel.
    int connected_;
    //! If true, the subscribtions are actually registered at the event channel.
    bool subscribed_;

    //! Vector of subscribed event types
    CosNotification::EventTypeSeq subscriptions_;
    //! Vector matching subscriptions to offered events.
    OfferVector offers_;
  };

  inline
  bool
  StructuredPushConsumer::connected() const
  {
    return connected_;
  }
  inline
  bool
  StructuredPushConsumer::subscribed() const
  {
    return subscribed_;
  }
  /**
   * @param index The index of the event in the subscription vector.
   * This index is returned as a vector from
   * addSubscripitons. Subscriptions specified as constructor the
   * argument subscription are indexed in ascending order, starting
   * with 0.
   */
  inline
  bool
  StructuredPushConsumer::offered(unsigned long _index) const
  {
    MIRO_ASSERT(_index < offers_.size());
    Guard guard(connectedMutex_);
    return offers_[_index];
  }

  inline
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr
  StructuredPushConsumer::getProxySupplier()
  {
    return proxySupplier_.in();
  }
}
#endif
