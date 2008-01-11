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
#ifndef SparrowCMPS03Impl_h
#define SparrowCMPS03Impl_h

#include "idl/CompassEventC.h"
#include "miro/CompassImpl.h"


#include "idl/CompassEventC.h"

#include "sparrow/Parameters.h"
#include "miro/Synch.h"
#include "miro/Thread.h"
#include <cmath>

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;
}

namespace Sparrow
{
  // forward declaration
  class CMPS03Impl;

  class CompassDispatcher : public Miro::Thread
  {
    typedef Miro::Thread Super;
  public:
    CompassDispatcher(Miro::StructuredPushSupplier * _supplier);
    int svc();
    void cancel(bool _wait = true);

    void setData(const Miro::CompassEventIDL& _data);
    void dispatch();

  protected:
    //! Supplier for events.
    Miro::StructuredPushSupplier * supplier_;

    //! Lock
    Miro::Mutex mutex_;
    //! Condition for triggering the dispatcher thread.
    Miro::Condition cond_;
    //! Preinitialized data structure for CMPS03 event.
    CosNotification::StructuredEvent notifyEvent_;

    //! Timeout for dispatching thread condition.
//     static ACE_Time_Value maxWait_;

    friend class CMPS03Impl;
  };

  //! Implementation of the CMPS03 interface.
  /**
   * This class offers a generic implementation for the CMPS03 module
   * interface. Simply pass the lowlevel sensor data through the
   * integrateData method and all the rest will be taken care for you.
   * Alternatively the four base classes' integrateData methods (with
   * their respective data structures) can be used separately.
   *
   * @author 
   */
  class CMPS03Impl :  public virtual Miro::CompassImpl
  {
  public:
    //! Initializing constructor.
    CMPS03Impl(	 Miro::StructuredPushSupplier * _supplier,
		 bool _asynchDispatching = true);
    virtual ~CMPS03Impl();

    //! Pass a CMPS03 reading from the device into CMPS03Impl.
    void integrateData(const Miro::CompassEventIDL& _data);
    void cancel();

    //! CMPS03 interface method implementation.
    virtual Miro::CompassEventIDL getData() throw();
    //! CMPS03 interface method implementation.
    virtual Miro::CompassEventIDL getWaitData() throw (Miro::ETimeOut);


  protected:
    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    //! Reference to CMPS03 parameters
//     const CMPS03Parameters& params_;
    //! Supplier for events.
    Miro::StructuredPushSupplier * supplier_;

    //! Lock
    Miro::Mutex mutex_;
    //! Condition for getWaitData
    Miro::Condition cond_;
    //! Last CMPS03 reading
    Miro::CompassEventIDL data_;

    //! Flag to indicate asynchronous event dispatching.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    CompassDispatcher dispatcherThread_;

    //! Timeout for getWaitData
    static ACE_Time_Value maxWait_;
  };
}

#endif // CMPS03Impl_h
