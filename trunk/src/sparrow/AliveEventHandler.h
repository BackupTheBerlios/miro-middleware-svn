// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef AliveEventHandler_h
#define AliveEventHandler_h

#include "AliveCollector.h"
#include "SparrowConnection2003.h"

#include "miro/StructuredPushSupplier.h"

#include <ace/Event_Handler.h>
#include <orbsvcs/CosNotifyCommC.h>

namespace Sparrow
{
  class AliveEventHandler : public ACE_Event_Handler
  {
  public:
    AliveEventHandler(AliveCollector* collector_, 
		      Connection2003 * connection_,
		      Miro::StructuredPushSupplier * pSupplier_);

    virtual int handle_timeout (const ACE_Time_Value &current_time, const void *act=0);
    
    
    Connection2003 * connection;
    AliveCollector * collector;
    Miro::StructuredPushSupplier * pSupplier;
    CosNotification::StructuredEvent notifyEvent;
  };
}

#endif


