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
#ifndef OdometryImpl_h
#define OdometryImpl_h

#include "OdometryS.h"

#include "Synch.h"

#include <orbsvcs/CosNotifyCommC.h>

namespace Miro 
{
  // forward decleration
  class StructuredPushSupplier;

  //Class OdometryImpl
  class  OdometryImpl : public virtual POA_Miro::Odometry
  {
  public:
    //Constructor 
    OdometryImpl(Miro::StructuredPushSupplier * _supplier,
		 bool _rawPositionEvents = true);
  
    //Destructor 
    virtual ~OdometryImpl();

    void integrateData(const MotionStatusIDL & data);
  
    virtual void setPosition(const Miro::PositionIDL & pos) throw();
    virtual void updatePosition(const Miro::PositionIDL & dPos) throw();
    virtual PositionIDL getPosition() throw();  
    virtual PositionIDL getWaitPosition() throw (ETimeOut);
    virtual PositionIDL getRawPosition() throw();  
    virtual PositionIDL getWaitRawPosition() throw (ETimeOut);
    virtual VelocityIDL getVelocity() throw();    
    virtual VelocityIDL getWaitVelocity() throw (ETimeOut);
    virtual MotionStatusIDL getStatus() throw();    
    virtual MotionStatusIDL getWaitStatus() throw (ETimeOut);

  protected:
    //-------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------

    // non - thread safe set postion stuff
    void protectedSetPosition(const PositionIDL& robot);
    void setTransformation();

    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------
    StructuredPushSupplier * supplier_;
    bool rawPositionEvents_;

    Mutex mutex_;
    Condition cond_;
    MotionStatusIDL status_;
    CosNotification::StructuredEvent notifyEvent_;
    CosNotification::StructuredEvent notifyRawEvent_;

    PositionIDL origin_;
    PositionIDL position_;

    double sinHeading_, cosHeading_;

    static ACE_Time_Value maxWait_;
  };

  //---------------------------------------------------------------------------
  // protected methods
  //---------------------------------------------------------------------------
  inline
  void
  OdometryImpl::setTransformation() {
    sinHeading_ = sin(origin_.heading);
    cosHeading_ = cos(origin_.heading);
  }
};
#endif // odometryImpl_hh
