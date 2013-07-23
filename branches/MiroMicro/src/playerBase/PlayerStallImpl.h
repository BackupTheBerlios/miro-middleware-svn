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
#ifndef PlayerStallImpl_h
#define PlayerStallImpl_h

#include "idl/StallS.h"

#include <orbsvcs/CosNotifyCommC.h>

// forward declarations
namespace Miro
{
  class StructuredPushSupplier;
};

namespace Player
{
  // forward declerations

  class BaseConnection;

  class StallImpl :  public virtual POA_Miro::Stall
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    StallImpl(Miro::StructuredPushSupplier * _pSupplier=NULL);
    virtual ~StallImpl();

    void pushEvent() const;

    //-------------------------------------------------------------------------
    // Stall interface
    //-------------------------------------------------------------------------
    virtual CORBA::Boolean isStalled() throw();
    virtual void unstall() throw();

    void integrateData(bool stalled);

  protected:
    bool stalled_;

    Miro::StructuredPushSupplier * pSupplier;
    CosNotification::StructuredEvent notifyEvent;
  };
}

#endif // PlayerStallImpl_h


