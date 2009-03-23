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
//
// Authors: 
//   Hans Utz
//   Philipp Baer
//
#ifndef NotifyMulticastAdapter_h
#define NotifyMulticastAdapter_h

/* TAO Notification includes */
#include <orbsvcs/CosNotifyChannelAdminC.h>
#include <orbsvcs/CosNotifyCommC.h>

/* Miro includes */
#include "miro/NotifyMulticastParameters.h"
#include "Exception.h"
#include "Client.h"

/* g++lib includes */
#include <string>
#include <map>

// forward declarations
class ACE_Reactor;
class ACE_SOCK_Dgram_Mcast;

namespace Miro 
{
  namespace NMC 
  {

    // forward declarations

    class SH;
    class TimeoutHandler;
    class EventHandler;
    class Sender;
    class Receiver;
    class EventFilter;

    /**
     * class Adapter
     *
     * Sets up a multicast gateway: events sent over the eventchannel
     * are multicasted to the peers in the multicast group.
     *
     * Since multicast is a very efficient way for group
     * communication, this is a very robust and simple way for
     * multi-agent scenarios  like RoboCup robot soccer games.
     *
     * Network connections via Wireless Lan (e.g. IEEE 802.11) are not
     * as reliable as wired networks. Thus, a connectionless protocol
     * like UDP is very useful.
     *
     * The datagrams are sent once and every peer that subscribed the
     * multicast group takes a copy of this packet. So the capacity
     * costs are kept very low.
     *
     * An ip multicast interface already is defined in TAO for the
     * Real-Time EventChannel. I've implemented an own version for
     * Miro, since Miro uses the Notification EventChannel.
     *
     * Subscribes the IP/MC group given in _multicastAddress. If
     * _multicastAddress is omitted, 225.2.2.1 is used as the default
     * MC-Group.  A connection to the EventChannel given by
     * _eventChannel is established, the necessary events are
     * subscribed and arriving events are pushed into this
     * EventChannel
     */

    class Adapter 
    {

    public:
      //! Initializing constructor.
      Adapter(int& _argc,
	      char *_argv[],
	      Miro::Client * _client,
	      CosNotifyChannelAdmin::EventChannel_ptr _eventChannel,
	      std::string& _domainName,
	      Parameters * _parameters = Parameters::instance())
	throw(CORBA::Exception, Miro::Exception);

      //! Destructor
      ~Adapter() throw();

      void init();
      void fini();

      void setEventFilter(EventFilter * _event_filter);
      void changeSubscription(const CosNotification::EventTypeSeq & added,
			      const CosNotification::EventTypeSeq & removed
			      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    protected:
      Parameters * parameters_;
      ACE_Reactor * reactor_;
      ACE_SOCK_Dgram_Mcast * socket_;

      Sender * sender_;
      Receiver * receiver_;

      /* event handling */
      int eventHandlerId_;
      EventHandler * eventHandler_;

      /* timeout handling */
      int timeoutHandlerId_;
      TimeoutHandler * timeoutHandler_;
      ACE_Time_Value timeoutHandlerInterval_;

      int shId_;
      SH * sh_;
      ACE_Time_Value shInterval_;
    };
  }
}
#endif // NotifyMulticastAdapter_h 
