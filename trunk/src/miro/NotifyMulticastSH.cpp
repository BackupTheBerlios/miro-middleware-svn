// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "NotifyMulticastSH.h"

#include <tao/Exception.h>

namespace Miro 
{
  namespace NMC
  {
    SH::SH(Sender *_sender, Receiver *_receiver, Config *_config) :
      sender_( _sender),
      receiver_(_receiver),
      config_(_config)
    {
    }

    SH::~SH() 
    {
    }

    int
    SH::handle_timeout(ACE_Time_Value const& /*_tv*/, void const * /*_act*/) 
    {
      CosNotification::StructuredEvent oe;
      CosNotification::StructuredEvent se;
      ::CosNotification::EventTypeSeq_var oets = 
	  sender_->proxySupplier_->obtain_offered_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF);
      ::CosNotification::EventTypeSeq_var sets = 
	  receiver_->proxyConsumer_->obtain_subscription_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF);

      /* only take non-local types for subscription list */
      std::vector<CosNotification::EventType *> sublist;
      sublist.reserve(sets->length());
      for (unsigned int i = 0; i < sets->length(); i++) {
	if (strcmp((*sets)[i].domain_name, receiver_->domainName().c_str())) {
	  sublist.push_back(&(*sets)[i]);
	}
      }

      sets->length(sublist.size());
      for (unsigned int i = 0; i < sublist.size(); i++) {
	(*sets)[i] = *sublist[i];
      }

      oe.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(receiver_->domainName().c_str());
      oe.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("NotifyMulticast::offered");
      oe.remainder_of_body <<= oets._retn();
            
      se.header.fixed_header.event_type.domain_name =
	CORBA::string_dup(receiver_->domainName().c_str());
      se.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("NotifyMulticast::subscribed");
      se.remainder_of_body <<= sets._retn();

      sender_->push_structured_event(oe);
      sender_->push_structured_event(se);

      /* subscription management */
      for (SubscribedMap::iterator itr = subscribedMap.begin();
	   itr != subscribedMap.end();
	   itr++) {
	itr->second--;

	/* check for outdated entry */
	if (itr->second == 0) {

	  try {
	    sender_->unsubscribe(receiver_->domainName().c_str(), itr->first);
	  } catch (CORBA::Exception const& e) {
	    MIRO_LOG_OSTR(LL_ERROR,
			  "Miro::NMC:SH: Uncaught exception unsubscribing " << itr->first <<
			  std::endl << e);
	  }
	  subscribedMap.erase(itr);
	}
      }

      /* offer management */
      int invalidated = 0;
      for (OfferMap::iterator itr = offerMap.begin();
	   itr != offerMap.end();
	   itr++) {
	itr->second--;

	/* check for outdated entry */
	if (itr->second == 0) {
	  offerMap.erase(itr);
	  invalidated++;
	}
      }

      if (invalidated > 0) {
	int pos = 0;
	CosNotification::EventTypeSeq ets(offerMap.size());

	for (OfferMap::iterator itr = offerMap.begin(); itr != offerMap.end(); itr++) {
	  ets[pos++].domain_name = CORBA::string_dup(itr->first.first.c_str());
	  ets[pos++].type_name = CORBA::string_dup(itr->first.second.c_str());
	}
                
	receiver_->setOffers(ets);
      }

      return 0;
    }
        
    void SH::handleOffers(CosNotification::EventTypeSeq const& ets) 
    {
      std::pair<std::string, std::string> val;
      for (unsigned int i = 0; i < ets.length(); i++) {
                
	val.first = ets[i].domain_name;
	val.second = ets[i].type_name;

	offerMap[val] = DEFAULT_LIVETIME;
      }
            
      receiver_->addOffers(ets);
    }

    void SH::handleSubscriptions(CosNotification::EventTypeSeq const& ets) 
    {
      for (unsigned int i = 0; i < ets.length(); i++) {
	if (!strcmp(ets[i].domain_name, receiver_->domainName().c_str())) {
	  SubscribedMap::iterator itr = 
	    subscribedMap.find((const char *)ets[i].type_name);

	  /* not yet subscribed */
	  if (itr == subscribedMap.end()) {
                    
	    try {
	      sender_->subscribe(receiver_->domainName().c_str(), 
				 (const char *)ets[i].type_name);
	      subscribedMap[(const char *)ets[i].type_name] = DEFAULT_LIVETIME;
                        
	    } catch (CORBA::Exception const& e) {
	      MIRO_LOG_OSTR(LL_ERROR, "Miro::NMC:SH: Uncaught exception subscribing for " << 
			    ets[i].domain_name << std::endl <<
			    e);
	    }
	  } else {
	    /* renew livetime */
	    subscribedMap[(const char *)ets[i].type_name] = DEFAULT_LIVETIME;
	  }
	}
      }
    }
  }
}

