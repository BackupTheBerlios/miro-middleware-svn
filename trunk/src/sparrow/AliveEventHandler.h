// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Sparrow_AliveEventHandler_h
#define Sparrow_AliveEventHandler_h

#include <ace/Event_Handler.h>
#include <orbsvcs/CosNotifyCommC.h>

// forward declarations
namespace Miro {
  class StructuredPushSupplier;
}

namespace Sparrow
{
  // forward declarations
  class Connection2003;
  class AliveCollector;

  //! Timeout handler to write periodic alive events.
  /** Maybe a thread would be better for decoupling? */
  class AliveEventHandler : public ACE_Event_Handler
  {
    //! Super class accessor.
    typedef ACE_Event_Handler Super;

  public:
    //! Initializing constructor.
    AliveEventHandler(AliveCollector* collector_, 
		      Connection2003 * connection_,
		      Miro::StructuredPushSupplier * pSupplier_);
    
    //! Inherited method.
    virtual int handle_timeout (const ACE_Time_Value &current_time,
				const void *act=0);
    
  protected:
    Connection2003 * connection_;
    AliveCollector * collector_;

    Miro::StructuredPushSupplier * pSupplier_;
    CosNotification::StructuredEvent notifyEvent_;
  };
}

#endif // Sparrow_AliveEventHandler_h


