#ifndef NotifyMulticastSH_h
#define NotifyMulticastSH_h

#include <ace/Event_Handler.h>
#include "NotifyMulticastSender.h"
#include "NotifyMulticastReceiver.h"
#include "NotifyMulticastConfig.h"

namespace Miro {

    namespace NotifyMulticast {

        class SH : public ACE_Event_Handler {

            public:
                SH(
                        Sender *_sender,
                        Receiver *_receiver,
                        Config *_config);
                ~SH();

                virtual int handle_timeout(
                    const ACE_Time_Value &_tv,
                    const void *_act);

                void handleOffers(CosNotification::EventTypeSeq &ets);

                void handleSubscriptions(CosNotification::EventTypeSeq &ets);
            
            protected:
                static const int DEFAULT_LIVETIME = 10;

                Sender *sender_;
                Receiver *receiver_;
                Config *config_;
                typedef std::map<std::string, int> SubscribedMap;
                SubscribedMap subscribedMap;
        };

    };
};

#endif /* NotifyMulticastSH_h */

