#include "NotifyMulticastSH.h"

namespace Miro {

    namespace NotifyMulticast {

        SH::SH(
                Sender *_sender,
                Receiver *_receiver,
                Config *_config) {
            sender_ = _sender;
            receiver_ = _receiver;
            config_ = _config;
            std::cout << "NotifyMulticast: DEBUG SH::SH" << std::endl;
        }


        SH::~SH() {
            std::cout << "NotifyMulticast: DEBUG SH::~SH" << std::endl;
        }


        int SH::handle_timeout(
            const ACE_Time_Value & /*_tv*/,
            const void * /*_act*/) {
            std::cout << "NotifyMulticast: DEBUG SH::handle_timeout" << std::endl;
            std::cout << "NotifyMulticast: sending offered/subscribed events" << std::endl;
            CosNotification::StructuredEvent oe;
            CosNotification::StructuredEvent se;
            ::CosNotification::EventTypeSeq_var oets = sender_->proxySupplier_->obtain_offered_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF);
            ::CosNotification::EventTypeSeq_var sets = receiver_->proxyConsumer_->obtain_subscription_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF);

            /* only take non-local types for subscription list */
            std::vector<std::string> sublist;
            std::cout << "NotifyMulticast: Subscription list:" << std::endl;
            for (unsigned int i = 0; i < sets->length(); i++) {
                if (strcmp((*sets)[i].domain_name, config_->getDomain().c_str())) {
                    std::cout << "NotifyMulticast: + " << (*sets)[i].domain_name << "/" << (*sets)[i].type_name << std::endl;
                    sublist.push_back((const char *)(*sets)[i].type_name);
                }
            }

            /* set alien types */
            sets->length(sublist.size());
            for (unsigned int i = 0; i < sublist.size(); i++) {
                (*sets)[i].domain_name = CORBA::string_dup(config_->getDomain().c_str());
                (*sets)[i].type_name = CORBA::string_dup(sublist[i].c_str());
            }

            /* print offerd list */
            std::cout << "NotifyMulticast: Offered list:" << std::endl;
            for (unsigned int i = 0; i < oets->length(); i++) {
                std::cout << "NotifyMulticast: + " << (*oets)[i].domain_name << "/" << (*oets)[i].type_name << std::endl;
            }
            
            oe.header.fixed_header.event_type.domain_name = CORBA::string_dup(config_->getDomain().c_str());
            oe.header.fixed_header.event_type.type_name = CORBA::string_dup("NotifyMulticast::offered");
            oe.remainder_of_body <<= oets._retn();
            
            se.header.fixed_header.event_type.domain_name = CORBA::string_dup(config_->getDomain().c_str());
            se.header.fixed_header.event_type.type_name = CORBA::string_dup("NotifyMulticast::subscribed");
            se.remainder_of_body <<= sets._retn();

            sender_->push_structured_event(oe);
            sender_->push_structured_event(se);

            for (SubscribedMap::iterator itr = subscribedMap.begin();
                 itr != subscribedMap.end();
                 itr++) {
                itr->second--;

                /* check for outdated entry */
                if (itr->second == 0) {
                    std::cout << "NotifyMulticast: Outdated subscription found for " << itr->first << std::endl;

                    try {
                        sender_->unsubscribe(config_->getDomain().c_str(), itr->first);
                    } catch (...) {
                        std::cerr << "NotifyMulticast: Uncaught exception unsubscribing " << itr->first << std::endl;
                    }
                    subscribedMap.erase(itr);
                }
            }

            return 0;
        }
        
        void SH::handleOffers(CosNotification::EventTypeSeq &/*ets*/) {
            std::cout << "NotifyMulticast: DEBUG SH::handleOffers" << std::endl;
            /* nothing done yet; TO BE IMPLEMENTED */
        }

        void SH::handleSubscriptions(CosNotification::EventTypeSeq &ets) {
            std::cout << "NotifyMulticast: DEBUG SH::handleSubscriptions" << std::endl;

            for (unsigned int i = 0; i < ets.length(); i++) {
                if (!strcmp(ets[i].domain_name, config_->getDomain().c_str())) {
                    SubscribedMap::iterator itr = subscribedMap.find((const char *)ets[i].type_name);

                    /* not yet subscribed */
                    if (itr == subscribedMap.end()) {
                        std::cout << "NotifyMulticast: subscription for " << ets[i].type_name << " requested" << std::endl;
                    
                        try {
                            sender_->subscribe(config_->getDomain().c_str(), (const char *)ets[i].type_name);
                            subscribedMap[(const char *)ets[i].type_name] = DEFAULT_LIVETIME;
                        
                        } catch (...) {
                            std::cerr << "NotifyMulticast: Uncaught exception subscribing for " << ets[i].domain_name << std::endl;
                        }
                    } else {
                        /* renew livetime */
                        subscribedMap[(const char *)ets[i].type_name] = DEFAULT_LIVETIME;
                    }
                }
            }
        }
    };
};

