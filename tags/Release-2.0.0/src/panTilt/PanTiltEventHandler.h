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
#ifndef PanTiltEventHandler_h
#define PanTiltEventHandler_h

#include "miro/DevEventHandler.h"
#include "miro/Synch.h"

namespace DpPanTilt
{
  // forward declarations
  class Connection;
  class Consumer;
  class Message;

  /**
   * Event is an object wrapper for the event handler 
   * registered with the reactor.
   *
   * it is provided with a log facility and inherits from 
   * ACE_Event_Handler
   */
  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;
  public:
    /**
     * constructor does initialization
     */
    EventHandler(Connection& _connection, Consumer * _consumer);

    /**
     * destructor, needed since virtual functions are used
     */  
    virtual ~EventHandler();

    /**
     * handles file descriptor events
     */
    virtual int handle_input (ACE_HANDLE fd); // fd select
    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  protected:

    /**
     * we need a reference to the module that handles handles
     * the connection to the pantilt device.
     */
    Connection& connection_;

    /**
     * The message block is allocated and filled here, it is destructed 
     * in panTiltTask after the packet is processed.
     */
    Message * msg_;

    /**
     * states for state machine
     */
    enum { WFSTAR, WFDELIM };

    /**
     * state of the state machine
     */
    int state;

    /**
     * size of the read buffer for reiceiving packets
     * is not really relevant, packets are short
     */ 
    static const size_t BUFFSIZE=256;
  };
};
#endif




