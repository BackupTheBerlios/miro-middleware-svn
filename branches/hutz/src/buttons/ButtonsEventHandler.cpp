// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "ButtonsEventHandler.h"
#include "ButtonsDevice.h"
#include "ButtonsConnection.h"

#include "miro/B21ButtonsC.h"
#include "miro/Exception.h"
#include "miro/TimeHelper.h"

#undef DEBUG

#ifdef DEBUG
#include <iostream>
using std::cout;
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace B21Buttons
{
  //
  // Constructors / Destructors
  //

  EventHandler::EventHandler(Connection& _connection, Miro::StructuredPushSupplier& _supplier) :
    connection(_connection),
    supplier_(_supplier),
    buttonStatus(0x00),
    blinkOn(false)
  {
    DBG(cout << "Constructing B21Buttons::EventHandler" << endl);

    // init button states
    for (int i = 0; i < NUM_BUTTONS; ++i) {
      button[i].id = i;
      button[i].event = Miro::Button::ON_RELEASE;
      button[i].mode = Miro::B21Button::TOGGLE_OFF;
    }

    // Status Notify Event initialization
    notifyEvent.header.fixed_header.event_type.domain_name = 
      CORBA::string_dup(supplier_.domainName().c_str());
    notifyEvent.header.fixed_header.event_type.type_name = 
      CORBA::string_dup("B21ButtonStatusIDL");
    notifyEvent.header.fixed_header.event_name = CORBA::string_dup("");
    notifyEvent.header.variable_header.length(0);   // put nothing here
    notifyEvent.filterable_data.length(0);          // put nothing here
  }
  
  EventHandler::~EventHandler()
  {
    DBG(cout << "Destructing B21Buttons::EventHandler" << endl);
  } 
  
  // timer callback
  int
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *arg) 
  {
    // DBG(cout << "B21Buttons timer: timeout" << endl);

    switch ((const int)arg) {
    case POLLING:
      pollButtons();
      break;
    case BLINKING:
      blinkButtons();
      break;
    default: 
      throw Miro::Exception("buttonEvent: Unknown timeout type.");
    }
    return 0;
  }

  //---------------------------------------------------------------------------
  // protected methods
  //---------------------------------------------------------------------------

  // poll current button press status
  void
  EventHandler::pollButtons() throw (Miro::EDevIO)
  {
    unsigned char status = connection.getButtonStatus();

    // only process if anything changed
    if (status == buttonStatus) 
      return;

    unsigned char lightBits = connection.getButtonLights();
    unsigned char light = RED_BIT;
    Miro::Button::Event event;
    ACE_Time_Value time = ACE_OS::gettimeofday();

    bool toggleLightStatus;
    //    CORBA::Any any;

    for (int i = 0; i < 4; ++i, light <<= 1) {     // for each button
      event = (status & light)? 
	Miro::Button::ON_PRESS 
	: 
	Miro::Button::ON_RELEASE;
      if (event != button[i].event) { 
	toggleLightStatus = false;                // press state changed
      
	Miro::timeA2C(time, button[i].time);
        button[i].event = event;  

	// adjust button mode
	if (button[i].event == Miro::Button::ON_PRESS) { 
	  switch (button[i].mode) {
	  case Miro::B21Button::FLASHING_TILL_PRESSED:
	    button[i].mode = Miro::B21Button::OFF;
	    if (connection.getButtonLights() & light)
	      toggleLightStatus = true;
	    break;
	  case Miro::B21Button::ON_TILL_PRESSED:
	    button[i].mode = Miro::B21Button::OFF;
	    toggleLightStatus = true;
	    break;
	  case Miro::B21Button::OFF_TILL_PRESSED:
	    button[i].mode = Miro::B21Button::ON;
	    toggleLightStatus = true;
	    break;
	  case Miro::B21Button::TOGGLE_ON:
	    button[i].mode = Miro::B21Button::TOGGLE_OFF;
	    toggleLightStatus = true;
	    break;
	  case Miro::B21Button::TOGGLE_OFF:
	    button[i].mode = Miro::B21Button::TOGGLE_ON;
	    toggleLightStatus = true;
	    break;
	  case Miro::B21Button::ON_WHILE_PRESSED:
	  case Miro::B21Button::OFF_WHILE_PRESSED:
	    toggleLightStatus = true;
	    break;
	  default:
	    break;
	  }
	} 
	else if (button[i].mode == Miro::B21Button::ON_WHILE_PRESSED || 
		 button[i].mode == Miro::B21Button::OFF_WHILE_PRESSED) {
	  toggleLightStatus = true;
	}
      
	// set the new light status
	if (toggleLightStatus)
	  if (lightBits & light) 
	    lightBits &= ~light;
	  else
	    lightBits |= light;
      }

    }
    // write lights 
    connection.setButtonLights(lightBits);

    // broadcast the changes
    light = RED_BIT;
    for (int j = 0; j < 4; ++j, light <<= 1) {     // for each button
      if ((buttonStatus & light) ^ (status & light)) {
	notifyEvent.remainder_of_body <<= button[j];
	supplier_.sendEvent(notifyEvent);
      }    
    }
    buttonStatus = status;
  }

  void
  EventHandler::blinkButtons() throw (Miro::EDevIO)
  {
    unsigned char lightBits = connection.getButtonLights();
    unsigned char light = RED_BIT;

    blinkOn = !blinkOn;
    for (int i = 0; i < NUM_BUTTONS; ++i, light <<= 1) {
      if (button[i].mode == Miro::B21Button::FLASHING ||
	  button[i].mode == Miro::B21Button::FLASHING_TILL_PRESSED) {
	if (blinkOn) 
	  lightBits &= ~light;
	else
	  lightBits |= light;
      }
    }
    connection.setButtonLights(lightBits);
  }
};
