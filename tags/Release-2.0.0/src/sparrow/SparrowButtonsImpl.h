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
