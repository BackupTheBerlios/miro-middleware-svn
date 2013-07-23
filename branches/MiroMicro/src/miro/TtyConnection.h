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
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#ifndef TtyConnection_h
#define TtyConnection_h

#include <ace/DEV_Connector.h>
#include <ace/TTY_IO.h>

// forward declarations
class ACE_Reactor;

namespace Miro
{
  // forward declarations
  class DevEventHandler;
  class TtyParameters;

  /**
   * The base class for a device handler.
   *
   * This is the
   */
  class TtyConnection
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
    TtyConnection(ACE_Reactor * _reactor,
                  DevEventHandler * _eventHandler,
                  const TtyParameters& _parameters);
    /**
     * Destructor.
     *
     * It deregisters the event handler from the reactor,
     * stops the consumer task and closes the file descriptor.
     */
    virtual ~TtyConnection();
    ACE_TTY_IO ioBuffer;

  protected:
    /** Pointer to the ACE_Reactor the @ref Event is registered to. */
    ACE_Reactor * reactor_;   // for event handling (file descriptors)
    /**
     * The consumer @ref DevEventHandler instance.
     */
    DevEventHandler * eventHandler_;
    ACE_DEV_Addr ttyName_;
    ACE_DEV_Connector connector_;
  };
}

#endif




