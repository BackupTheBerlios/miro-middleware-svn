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
#ifndef Sparrow_AliveEventHandler_h
#define Sparrow_AliveEventHandler_h

#include "miro/Thread.h"
#include "miro/Synch.h"
#include "idl/SparrowAliveC.h"

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
  class AliveEventHandler;

  class AliveDispatcher : public Miro::Thread
  {
    typedef Thread Super;
  public:
    AliveDispatcher(Miro::StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(const Miro::SparrowAliveIDL& _data);
    void dispatch();

  protected:
    //! Supplier for events.
    Miro::StructuredPushSupplier * supplier_;

    //! Lock
    Miro::Mutex mutex_;
    //! Condition for triggering the dispatcher thread.
    Miro::Condition cond_;
    //! Preinitialized data structure for Alive event.
    CosNotification::StructuredEvent notifyEvent_;

    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend class AliveEventHandler;
  };

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
    virtual ~AliveEventHandler();

    //! Inherited method.
    virtual int handle_timeout (const ACE_Time_Value &current_time,
				const void *act=0);
    
  protected:
    Connection2003 * connection_;
    AliveCollector * collector_;
    AliveDispatcher dispatcher_;

    Miro::StructuredPushSupplier * pSupplier_;
    CosNotification::StructuredEvent notifyEvent_;
  };
}

#endif // Sparrow_AliveEventHandler_h


