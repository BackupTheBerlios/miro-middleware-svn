// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004, 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef SparrowPanTiltImpl_h
#define SparrowPanTiltImpl_h

#include "miro/PanTiltImpl.h"
#include "idl/SparrowPanTiltS.h"
#include "miro/Synch.h"
#include "miro/Angle.h"

#include "sparrow/Parameters.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro
{
  class StructuredPushSupplier;
}

namespace Sparrow
{
  // forward declaration
  class BaseConnection;
  class Parameters;

  class  PanTiltImpl : public virtual POA_Miro::SparrowPanTilt,
		       public Miro::PanImpl
  {
  public:
    //Constructor
    PanTiltImpl(BaseConnection * _connection,
		Miro::PanParameters _panParameters,
		Miro::StructuredPushSupplier * _pSupplier = NULL);

    //Destructor
    virtual ~PanTiltImpl();

    virtual void setPan(CORBA::Double value)
      throw (Miro::EDevIO, Miro::EOutOfBounds);
    virtual CORBA::Double getPan() throw (Miro::EDevIO);

    virtual CORBA::Boolean panning(const Miro::TimeIDL& stamp) throw();
    virtual Miro::PanPositionIDL currentPan(const Miro::TimeIDL& stamp) throw();

  protected:
    bool prvPanning(const ACE_Time_Value& _t);
    Miro::PanPositionIDL currentPosition(const ACE_Time_Value& _t);

    BaseConnection *         connection;
    const Parameters&        params_;
    Miro::Mutex              mutex;

    Miro::StructuredPushSupplier * pSupplier_;
    CosNotification::StructuredEvent notifyEvent;

    double lastPosition;
    double nextPosition;
    ACE_Time_Value timeLastSet;

    ACE_Time_Value totalLatency;
    bool sparrow2003_;
  };

  inline
  bool
  PanTiltImpl::prvPanning(const ACE_Time_Value& _t)
  {
    if (timeLastSet < _t) {
      ACE_Time_Value t1 = _t; // elapsed time
      t1 -= timeLastSet;
      ACE_Time_Value t2; // time needed to do pan
      t2.set(fabs(nextPosition - lastPosition) / params_.panRadPerSec);
      t2 += totalLatency;

      return (params_.panLatency < t1 && t1 < t2);
    }
    else {
      return false;
    }
  }
}

#endif // SparrowPanTiltImpl_h
