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
#ifndef SparrowStallImpl_h
#define SparrowStallImpl_h

#include "idl/StallS.h"

#include <orbsvcs/CosNotifyCommC.h>

// forward declerations
namespace Miro
{
  class StructuredPushSupplier;
};

namespace Sparrow
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
    StallImpl(BaseConnection * _connection,
	      Miro::StructuredPushSupplier * _pSupplier);
    virtual ~StallImpl();

    void pushEvent() const;

    //-------------------------------------------------------------------------
    // Stall interface
    //-------------------------------------------------------------------------
    virtual CORBA::Boolean isStalled() throw();
    virtual void unstall() throw();

  protected:
    BaseConnection * connection_; // encapsulating communication to hardware
    Miro::StructuredPushSupplier * pSupplier;
    CosNotification::StructuredEvent notifyEvent;
  };
}

#endif // SparrowStallImpl_h


