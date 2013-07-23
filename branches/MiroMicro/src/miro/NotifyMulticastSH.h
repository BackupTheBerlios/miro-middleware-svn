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
#ifndef NotifyMulticastSH_h
#define NotifyMulticastSH_h

#include "NotifyMulticastSender.h"
#include "NotifyMulticastReceiver.h"

#include <ace/Event_Handler.h>

#include <map>

namespace Miro
{
  namespace NMC {
    class SH : public ACE_Event_Handler
    {
    public:
      SH(Sender *_sender,
         Receiver *_receiver);
      virtual ~SH();

      virtual int handle_timeout(ACE_Time_Value const& _tv, void const *_act);

      void handleOffers(CosNotification::EventTypeSeq const& ets);
      void handleSubscriptions(CosNotification::EventTypeSeq const& ets);

    protected:
      static const int DEFAULT_LIVETIME = 10;

      Sender *sender_;
      Receiver *receiver_;

      typedef std::map<std::string, int> SubscribedMap;
      typedef std::map<std::pair<std::string, std::string> , int> OfferMap;
      SubscribedMap subscribedMap;
      OfferMap offerMap;
    };
  }
}
#endif // NotifyMulticastSH_h

