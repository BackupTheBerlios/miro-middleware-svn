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
#ifndef RangeSensorImpl_h
#define RangeSensorImpl_h

#include "RangeSensorS.h"
#include "Synch.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  // forward declaration
  class StructuredPushSupplier;

  class RangeSensorImpl : virtual public POA_Miro::RangeSensor
  {
  public:
    RangeSensorImpl(const ScanDescriptionIDL& _description,
		    StructuredPushSupplier * _supplier = NULL);
    ~RangeSensorImpl();

    virtual void integrateData(RangeScanEventIDL * _data);
    virtual void integrateData(RangeGroupEventIDL * _data);
    virtual void integrateData(RangeBunchEventIDL * _data);

    // IDL interface

    ScanDescriptionIDL * getScanDescription() throw();
    RangeGroupEventIDL * getGroup(CORBA::Long id) throw();
    RangeGroupEventIDL * getWaitGroup(CORBA::Long id) throw(ETimeOut);
    RangeScanEventIDL * getFullScan() throw();
    RangeScanEventIDL * getWaitFullScan() throw(ETimeOut);

  protected:
    StructuredPushSupplier * supplier_;
    const ScanDescriptionIDL& description_;
    bool masking_;

    Mutex mutex_;
    Condition condition_;
    RangeScanEventIDL scan_;
    CosNotification::StructuredEvent notifyEvent;

    static ACE_Time_Value maxWait;
  };
};

#endif // rangeSensorImpl_hh
