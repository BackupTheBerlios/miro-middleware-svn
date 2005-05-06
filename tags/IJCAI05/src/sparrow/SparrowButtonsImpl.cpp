// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "SparrowButtonsImpl.h"
#include "SparrowConsumer.h"

#include "idl/ExceptionC.h"
#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"

namespace Sparrow
{
  ButtonsImpl::ButtonsImpl(Miro::StructuredPushSupplier * _pSupplier) :
    pSupplier(_pSupplier)
  {
    MIRO_LOG_CTOR("ButtonsImpl");

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

      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent.header.fixed_header.event_type;
      pSupplier->addOffers(offers);
    }
  }
    
  ButtonsImpl::~ButtonsImpl()
  {
    MIRO_LOG_DTOR("ButtonsImpl");
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
}
