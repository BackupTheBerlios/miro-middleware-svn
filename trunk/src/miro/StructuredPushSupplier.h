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
#ifndef miroSturcturedPushSupplier_h
#define miroSturcturedPushSupplier_h

#include "Synch.h"
#include "Log.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

#include <string>
#include <vector>

namespace Miro
{
  //! StructuredPushSupplier interface implementation.
  /**
   * This class implements the StructuredPushSupplier interface of the
   * CORBA notification service. It performs bookkeeping of
   * subscriptions from consumers to the events offered by this
   * supplier and allowes efficient querying of this information for
   * event producers.
   */
  class StructuredPushSupplier : public POA_CosNotifyComm::StructuredPushSupplier
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
    StructuredPushSupplier(CosNotifyChannelAdmin::EventChannel_ptr  _ec,
			   std::string const& _domainName = std::string(),
			   bool _connect = true,
			   CosNotification::EventTypeSeq const& _offer = 
			   CosNotification::EventTypeSeq());

    //! Destructor
    virtual ~StructuredPushSupplier();

    //! Connect to proxy consumer.
    void connect();
    //! Disconnect supplier to proxy consumer.
    void disconnect();

    //! Test whether an offered event is subscribed.
    bool subscribed(unsigned long _index) const;
    //! Test whether an offered event is subscribed.
    bool subscribed(std::string const& _domain, std::string const& _type) const;

    //! Add an offers to the notification channel.
    unsigned long addOffer(CosNotification::StructuredEvent const& _added);
    //! Add a set of offers to the notification channel.
    IndexVector addOffers(CosNotification::EventTypeSeq const& _added);
    //! Set the set of offers to the notification channel.
    void setOffers(CosNotification::EventTypeSeq const& _offers);

    //! Send one event.
    void sendEvent(const CosNotification::StructuredEvent& event);
    
    //! Accessor method for the domain name.
    std::string const& domainName() const;

    //--------------------------------------------------------------------------
    // public static methods
    //--------------------------------------------------------------------------
    static void initStructuredEvent(CosNotification::StructuredEvent& _event,
				    std::string const& _domainName,
				    std::string const& _typeName);
  protected:
    //--------------------------------------------------------------------------
    // protected types
    //--------------------------------------------------------------------------

    typedef std::vector<bool> SubscriptionVector;

    //--------------------------------------------------------------------------
    // protected methods
    //--------------------------------------------------------------------------

    // inherited IDL interfae

    //! CosNotifyComm::StructuredPushSupplier interface method implementation.
    virtual void subscription_change (const CosNotification::EventTypeSeq & added,
				      const CosNotification::EventTypeSeq & removed
				      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException, CosNotifyComm::InvalidEventType);

    //! CosNotifyComm::StructuredPushSupplier interface method implementation.
    virtual void disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      throw(CORBA::SystemException);

    //! Tell the admin about an offer change and update the subscription vector.
    void initiateOfferChange(CosNotification::EventTypeSeq const& _added,
			     CosNotification::EventTypeSeq const& _removed);

    //--------------------------------------------------------------------------
    // protected static methods
    //--------------------------------------------------------------------------

    //! Helper method to initialize the default offer.
    static CosNotification::EventTypeSeq asterixOffer();

    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------

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
    mutable Miro::Mutex connectedMutex_;
    //! If true, the supplier is connected to the event channel.
    bool connected_;

    //! Vector of offered event types
    CosNotification::EventTypeSeq offers_;
    //! Vector matching subscriptions to offered events.
    SubscriptionVector subscription_;
  };

  inline
  void
  StructuredPushSupplier::sendEvent(const CosNotification::StructuredEvent& event)
  {
    if (connected_) {
      proxyConsumer_->push_structured_event(event);
    }
  }


  /**
   * @param index The index of the event in the offer vector.  This
   * index is returned as a vector from addOffers. Offers specified as
   * constructor the argument offer are indexed in ascending order,
   * starting with 0.
   */
  inline
  bool
  StructuredPushSupplier::subscribed(unsigned long _index) const {
    MIRO_ASSERT(_index < subscription_.size());
    return subscription_[_index];
  }

  inline
  std::string const& 
  StructuredPushSupplier::domainName() const {
    return domainName_;
  }
}
#endif
