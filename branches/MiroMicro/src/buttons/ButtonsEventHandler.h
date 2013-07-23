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
#ifndef ButtonsEventHandler_h
#define ButtonsEventHandler_h

#include "idl/B21ButtonsC.h"
#include "miro/StructuredPushSupplier.h"
#include "ButtonsDevice.h"

#include <ace/Event_Handler.h>

// forward declarations
namespace Miro
{
  class StructuredPushSupplier;
};

namespace B21Buttons
{
  // forward declarations
  class Connection;

  //! Data type holding the button status in case of a button event. 
  /**
   * This data type is shipped via the notification channel
   * in the robots naming context on each press status change of a
   * button. 
   */
  struct ButtonStatus : public Miro::ButtonStatusIDL
  {
    Miro::B21Button::Mode mode;
  };
  
  /**
   * Event handler for buttons timer events.
   *
   * Since B21ButtonsConnection connects directly to the ports, there is
   * nothing we can seriously do but poll, to recognice a button press
   * or release. Therefor this event handler is only registerd for timer
   * events: One for polling of the buttons status and one to make the
   * buttons blink.
   */
  class EventHandler : public ACE_Event_Handler
  {
  public:
    /** 
     * Initializing constructor
     *
     * Takes a pointer to the io connection and
     * one to the event channel button status changes are pushed into.
     */
    EventHandler(Connection&  _connection, Miro::StructuredPushSupplier& _supplier);
    /** Ceaning up. */
    virtual ~EventHandler();
  
    /** ACE_Event_Handler interface. */
    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  protected:
    /** 
     * Handler for buttons status update.
     *
     * Reads the current buttons status, pushes status changes into the
     * event channel and changes the button modes apropriately.  
     */
    void pollButtons() throw(Miro::EDevIO);
    /**
     * Handler for button blinking.
     *
     * Buttons with the modes FLASHING or FLASHIN_TILL_PRESSED become
     * toggled on and of every half a second.
     */
    void blinkButtons() throw(Miro::EDevIO);

    Connection& connection;
    Miro::StructuredPushSupplier& supplier_;

    unsigned char buttonStatus;
    bool blinkOn;                    // blink status: on or off

  public:
    /** Status of the individual buttons. */
    //  Miro::Mutex buttonMutex;
    ButtonStatus button[NUM_BUTTONS];
    CosNotification::StructuredEvent notifyEvent;
  };
};
#endif
