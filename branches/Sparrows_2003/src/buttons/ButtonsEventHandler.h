// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef ButtonsEventHandler_h
#define ButtonsEventHandler_h

#include "miro/B21ButtonsC.h"
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
