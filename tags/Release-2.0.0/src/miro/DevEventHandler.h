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
#ifndef DevEventHandler_h
#define DevEventHandler_h

#include <ace/Event_Handler.h>

namespace Miro
{
  // forward declarations
  class DevConsumer;
  class DevMessage;

  /**
   * Event handling class.
   *
   * This class waits for input on the file descriptor, registered for
   * by @ref Connection and passes collected data into the message
   * queue of the associated @ref DevConsumer for further asynchronous
   * processing.
   *
   * Subclass this class for, and overwrite the handle_event method
   * inherited by ACE_Event_Handler. Get the pending data of the
   * file descriptor by read(), copy the data into the ACE_Message_Block
   * provided by this class and pass the message on to the consumer
   * task by a call to dispatchMessage().
   *
   * @author Hans Utz
   */
  class DevEventHandler : public ::ACE_Event_Handler
  {
    typedef ACE_Event_Handler Super;

  public:
    /**
     * Constructor
     *
     * @param consumer_ 
     * A pointer to the consumer who's message queue is to be filled 
     * by the event instance.
     *
     * @param messageSize
     * The size (sizeof()) of the message to be passed. The Event
     * provides a ACE_Message_Block with messageSize bytes in its 
     * message buffer to copy the message into. - Well, not type safe,
     * but the alternative would have been to templatize 
     * @ref Connection, @ref Event and @ref DevConsumer.
     */
    DevEventHandler(DevConsumer* _consumer, DevMessage* _message);
    /**
     * Destructor
     */
    virtual ~DevEventHandler();

    // inherited methods

    /**
     * Called when file descriptor is removed from the reactor.
     *
     * This implementation commits suicide and calls delete(this);
     */
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
    virtual void set_handle(ACE_HANDLE _handle);
    virtual ACE_HANDLE get_handle() const;

  protected:
    /**
     * Pass message on to the consumer
     *
     * This method passes the message on to the message queue of the
     * @ref DevConsumer instance and allocates a new one.
     */
    virtual void dispatchMessage();

    ACE_HANDLE handle_;

    /**
     * pointer to the consumer instance
     */
    DevConsumer* consumer_; 
    DevMessage* message_;
  };
}
#endif




