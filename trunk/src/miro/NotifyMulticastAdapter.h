// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast Adapter
//
//
//  (c) 2001, 2002, 2003, 2004
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <phbaer@npw.net>
//    Hans Utz <hans.utz@informatik.uni-ulm.de>
//
//
//  Version:
//    1.1.1
//
//
//  Description:
//
//    Sets up a multicast gateway: events sent over the eventchannel
//    are multicasted to the peers in the multicast group.
//
//    Since multicast is a very efficient way for group communication,
//    this is a very robust and simple way for multi-agent scenarios
//    like RoboCup robot soccer games.
//
//    Network connections via Wireless Lan (e.g. IEEE 802.11) are not
//    as reliable as wired networks. Thus, a connectionless protocol
//    like UDP is very useful.
//
//    The datagrams are sent once and every peer that subscribed the
//    multicast group takes a copy of this packet. So the capacity
//    costs are kept very low.
//
//    An ip multicast interface already is defined in TAO for the
//    Real-Time EventChannel. I've implemented an own version for Miro,
//    since Miro uses the Notification EventChannel.
//
//
//  Changes:
//
//    1.1.0
//    - added subscription protocol
//
//    1.0.4
//    - removed code that was commented out
//    - added support for logging debug output to a file
//
//    1.0.3
//    - many clean ups
//    - removed RtEC dependency
//    - changed to own namespace
//    - renamed class NotifyForward to Adapter
//    - removed some deprecated methods
//
//    1.0.2.pre
//    - Fixes for disabled ping
//    - Class added for configuration variables
//
//    1.0.1.experimental
//    - Different send/recv addresses possible
//    - Endpoint removed
//    - Multicast socket initialized in NotifyMulticast
//
//    1.0.0.experimental
//    - Initial release
//
//
//  Requires:
//    notifyMulticastSender.hh
//    notifyMulticastReceiver.hh
//    notifyMulticastEventHandler.hh
//    notifyMulticastTimeoutHandler.hh
//
//
//  Command Line Arguments: (case insensitive)
//    -MCastGroup <ip>:  Change default IP/MC Group
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef NotifyMulticastAdapter_h
#define NotifyMulticastAdapter_h

/* NotifyMulticast includes */
#include <miro/NotifyMulticastConfig.h>

/* TAO Notification includes */
#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

/* ACE includes */
#include <ace/Reactor.h>

/* Miro includes */
#include "Exception.h"
#include "Client.h"

/* g++lib includes */
#include <string>
#include <map>
#include <fstream>

namespace Miro {

    namespace NotifyMulticast {

        // forward declarations

        class SH;

        class TimeoutHandler;

        class EventHandler;

        class Sender;

        class Receiver;

        /**
         * class Adapter
         *
         *   Descritpion:
         *     Subscribes the IP/MC group given in _multicastAddress. If _multicastAddress
         *     is omitted, 225.2.2.1 is used as the default MC-Group.
         *     A connection to the EventChannel given by _eventChannel is established,
         *     the necessary events are subscribed and arriving events are pushed into
         *     this EventChannel
         */

        class Adapter {

            public:
                //! Initializing constructor.
                Adapter(
                    int _argc,
                    char *_argv[],
                    Miro::Client *_client,
                    CosNotifyChannelAdmin::EventChannel_ptr _eventChannel,
                    unsigned int _eventMaxAge = 500,
                    std::string _multicastAddress = "225.2.2.1")
                throw(CORBA::Exception, Miro::Exception);

                //! Destructor
                ~Adapter() throw(CORBA::Exception, Miro::Exception);

                void init();
                void fini();

                //! Returns pointer to sender.
                Sender *getSender();

                //! Returns pointer to receiver.
                Receiver *getReceiver();

                /* Fried classes */

                friend class Receiver;

                friend class Sender;

            protected:
                Miro::Client *client_;

                ACE_Reactor *reactor_;

                Config configuration_;

                Sender *sender_;
                Receiver *receiver_;

                /* event handling */
                int eventHandlerId_;
                EventHandler *eventHandler_;

                /* timeout handling */
                int timeoutHandlerId_;
                TimeoutHandler *timeoutHandler_;
                ACE_Time_Value timeoutHandlerInterval_;

                int shId_;
                SH *sh_;
                ACE_Time_Value shInterval_;

                /* logfile */
                std::ofstream logfile_;
                bool useLogfile_;

                ACE_Date_Time dt_;

                static const char months[13][4];

        };
    };
};

#endif /* NotifyMulticastAdapter_h */
