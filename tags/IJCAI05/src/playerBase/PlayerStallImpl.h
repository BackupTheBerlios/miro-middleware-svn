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


