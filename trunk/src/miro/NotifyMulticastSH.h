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
#ifndef NotifyMulticastSH_h
#define NotifyMulticastSH_h

#include "NotifyMulticastSender.h"
#include "NotifyMulticastReceiver.h"
#include "NotifyMulticastConfig.h"

#include <ace/Event_Handler.h>

namespace Miro 
{
  namespace NMC
  {
    class SH : public ACE_Event_Handler 
    {
    public:
      SH(Sender *_sender,
	 Receiver *_receiver,
	 Config *_config);
      virtual ~SH();

      virtual int handle_timeout(ACE_Time_Value const& _tv, void const *_act);

      void handleOffers(CosNotification::EventTypeSeq const& ets);
      void handleSubscriptions(CosNotification::EventTypeSeq const& ets);
            
    protected:
      static const int DEFAULT_LIVETIME = 10;

      Sender *sender_;
      Receiver *receiver_;
      Config *config_;
                
      typedef std::map<std::string, int> SubscribedMap;
      typedef std::map<std::pair<std::string, std::string> , int> OfferMap;
      SubscribedMap subscribedMap;
      OfferMap offerMap;
    };
  }
}
#endif // NotifyMulticastSH_h

