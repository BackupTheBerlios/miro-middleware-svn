// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast
//
//
//  (c) 2001, 2002, 2003, 2004
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <phbaer@openums.org>
//
//
//  Version:
//    1.0.3
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
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef NotifyMulticast_hh
#define NotifyMulticast_hh

#include "NotifyMulticastAdapter.h"
#include "NotifyMulticastDefines.h"

#endif /* NotifyMulticast_hh */
