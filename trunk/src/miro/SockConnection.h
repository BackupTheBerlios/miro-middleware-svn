// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 005: Defence Research and Development Canada, Suffield
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
// Authors: G. Broten
//
#ifndef SockConnection_h
#define SockConnection_h

/* ACE Includes */
#include <ace/DEV_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>

// forward declarations
class ACE_Reactor;

namespace Miro
{
  // forward declarations
  class DevEventHandler;
  class SockParameters;

  /**
   * The base class for a device handler.
   *
   * This is the
   */
  class SockConnection
  {
  public:
    /**
     * Constructor.
     *
     * It stores the reactor, event and task,
     * opens the file pointed to by deviceName
     * and registers the Event::handle_event method at the reactor.
     *
     * @param reactor_
     *        Pointer to a ACE_Reactor.
     * @param event_
     *        Pointer to an instance of your Event class, which is
     *        derived from @ref Event.
     * @param task_
     *        Pointer to an instance of your Task class, which is
     *        derived from @ref Task.
     * @param deviceName
     *        Name of the device to open.
     */
    SockConnection(ACE_Reactor * _reactor,
                   DevEventHandler * _eventHandler,
                   const SockParameters& _parameters);
    /**
     * Destructor.
     *
     * It deregisters the event handler from the reactor,
     * stops the consumer task and closes the file descriptor.
     */
    virtual ~SockConnection();


    /** Pointer to the ACE_Reactor the @ref Event is registered to. */
    ACE_Reactor * reactor_;   // for event handling (file descriptors)
    /**
     * The consumer @ref DevEventHandler instance.
     */
    DevEventHandler * eventHandler_;

    /*
    ** Define the connector classes
    */
    ACE_SOCK_Stream peer_;
    const char* ifName_;
    const char* hostName_;
    const char* portName_;
    ACE_INET_Addr srvr_;
    ACE_INET_Addr local_;
    ACE_SOCK_Connector connector_;
  };
}

#endif



