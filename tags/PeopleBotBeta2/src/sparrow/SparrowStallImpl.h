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
#ifndef SparrowStallImpl_h
#define SparrowStallImpl_h

#include "miro/StallS.h"
#include "miro/Exception.h"

#include <orbsvcs/CosNotifyCommC.h>

// forward declerations
class ACE_Reactor;
namespace Miro
{
  class StructuredPushSupplier;
};

namespace Sparrow
{
  // forward declerations
  class Connection;
  class Consumer;

  class StallImpl :  public virtual POA_Miro::Stall
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    StallImpl(Connection& _connection,
	      Miro::StructuredPushSupplier * _pSupplier)
      throw(Miro::Exception);
    virtual ~StallImpl();

    void pushEvent() const;

    //-------------------------------------------------------------------------
    // Stall interface
    //-------------------------------------------------------------------------
    virtual CORBA::Boolean isStalled() throw();
    virtual void unstall() throw();

  protected:
    Connection& connection; // encapsulating communication to hardware
    Miro::StructuredPushSupplier * pSupplier;
    CosNotification::StructuredEvent notifyEvent;
  };
};

#endif


