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
#ifndef SparrowButtonsImpl_h
#define SparrowButtonsImpl_h

#include "SparrowDevice.h"

#include "idl/ButtonsS.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;
}

namespace Sparrow
{
  // forward declerations
  class Consumer;

  class ButtonsImpl : public virtual POA_Miro::Buttons
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    ButtonsImpl(Miro::StructuredPushSupplier * _pSupplier);
    virtual ~ButtonsImpl();

    void pushEvent(Miro::ButtonStatusIDL * pEvent);

    //------------------------------------------------------------------------
    // Buttons interface
    //------------------------------------------------------------------------
    virtual CORBA::Boolean isPressed(CORBA::ULong id) throw(Miro::EOutOfBounds);
    virtual CORBA::ULong numberOfButtons() throw();

  protected:
    Miro::StructuredPushSupplier * pSupplier;
    CosNotification::StructuredEvent notifyEvent;

    CORBA::Boolean button_[NUMBER_OF_BUTTONS];
  };
}

#endif // SparrowButtonsImpl_h
