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
#ifndef SparrowButtonsImpl_h
#define SparrowButtonsImpl_h

#include "miro/ButtonsS.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;
};

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
    ButtonsImpl(Consumer& _consumer,
		Miro::StructuredPushSupplier * _pSupplier);
    virtual ~ButtonsImpl();

    void pushEvent(Miro::ButtonStatusIDL * pEvent);

    //------------------------------------------------------------------------
    // Buttons interface
    //------------------------------------------------------------------------
    virtual CORBA::Boolean isPressed(CORBA::Long id) throw(Miro::EOutOfBounds);
    virtual CORBA::Long numberButtons() throw();

  protected:
    Consumer& consumer;
    Miro::StructuredPushSupplier * pSupplier;
    CosNotification::StructuredEvent notifyEvent;
  };
};

#endif
