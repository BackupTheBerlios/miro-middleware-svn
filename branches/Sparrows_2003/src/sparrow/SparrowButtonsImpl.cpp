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


#include "SparrowButtonsImpl.h"
#include "SparrowConsumer.h"

#include "miro/ExceptionC.h"
#include "miro/StructuredPushSupplier.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Sparrow
{
  ButtonsImpl::ButtonsImpl(Consumer& _consumer,
			   Miro::StructuredPushSupplier * _pSupplier) :
    consumer(_consumer),
    pSupplier(_pSupplier)
  {
    DBG(cout << "Constructing SparrowButtonsImpl" << endl);

    // initialize button state
    for (unsigned long i = 0; i < NUMBER_OF_BUTTONS; ++i)
      button_[i] = false;

    // Stall Notify Event initialization
    if (pSupplier) {
      notifyEvent.header.fixed_header.event_type.domain_name = 
        CORBA::string_dup(pSupplier->domainName().c_str());
      notifyEvent.header.fixed_header.event_type.type_name = 
  	  CORBA::string_dup("Button");
      notifyEvent.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent.header.variable_header.length(0);   // put nothing here
      notifyEvent.filterable_data.length(0);          // put nothing here
    }
  }
    
  ButtonsImpl::~ButtonsImpl()
  {
    DBG(cout << "Destructing SparrowButtonsImpl" << endl);
  }

  void
  ButtonsImpl::pushEvent(Miro::ButtonStatusIDL * pEvent)
  {
    button_[pEvent->number] = (pEvent->event == Miro::Button::ON_PRESS);

    if (pSupplier) {
      notifyEvent.remainder_of_body <<= pEvent;
      pSupplier->sendEvent(notifyEvent);
    }
  }

  // 
  // IDL interface implementation
  CORBA::Boolean
  ButtonsImpl::isPressed(CORBA::ULong id) throw(Miro::EOutOfBounds)
  {
    if (id >= NUMBER_OF_BUTTONS)
      throw Miro::EOutOfBounds();

    return button_[id];
  }

  CORBA::ULong
  ButtonsImpl::numberOfButtons() throw()
  {
    return NUMBER_OF_BUTTONS;
  }
};
