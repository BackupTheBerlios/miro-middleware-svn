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
#include "Thread.h"
#include <cmath>

#include <orbsvcs/CosNotifyCommC.h>

#include <cmath> 

namespace Miro 
{
  // forward decleration
  class StructuredPushSupplier;
  class OdometryImpl;

  class OdometryDispatcher : public Thread
  {
    typedef Thread Super;
  public:
    OdometryDispatcher(Miro::StructuredPushSupplier * _supplier,
		       bool _rawPositionEvents = true);
    int svc();
    void cancel(bool _wait = true);

    void setData(const MotionStatusIDL& _status,
		 const RawPositionIDL& _raw);
    void dispatch();

  protected:
    //! Supplier for events.
    StructuredPushSupplier * supplier_;
    //! True if also RawPosition events shall be emitted.
    bool rawPositionEvents_;

    //! Lock
    Mutex mutex_;
    //! Condition for getWaitPosition etc.
    Condition cond_;
    //! Preinitialized data structure for Odometry event.
    CosNotification::StructuredEvent notifyEvent_;
    //! Preinitialized data structure for RawPosition events.
    CosNotification::StructuredEvent notifyRawEvent_;
    
    //! Timeout for dispatching thread condition.
    static ACE_Time_Value maxWait_;

    friend OdometryImpl;
  };

  //! Implementation of the Odometry interface.
  /**
   * This class offers a generic implementation for the Odometry
   * interface. Simply pass the lowlevel odometry sensor data through
   * the integrateData method and all the rest will be taken care for
   * you.
   *
   * @author Hans Utz
   */
  class  OdometryImpl : public virtual POA_Miro::Odometry
  {
  public:
    //! Initializing constructor.
    OdometryImpl(Miro::StructuredPushSupplier * _supplier,
		 bool _rawPositionEvents = true,
		 bool _asychDispatching = false);
    virtual ~OdometryImpl();

    //! Method to pass raw odometry data from the device into the OdometryImpl class.
    void integrateData(const MotionStatusIDL & data);
  
    //! Odometry interface method implementation.
    virtual void setPosition(const Miro::PositionIDL & pos) throw();
    //! Odometry interface method implementation.
    virtual void updatePosition(const Miro::PositionIDL & dPos) throw();
    //! Odometry interface method implementation.
    virtual PositionIDL getPosition() throw();  
    //! Odometry interface method implementation.
    virtual PositionIDL getWaitPosition() throw (ETimeOut);
    //! Odometry interface method implementation.
    virtual PositionIDL getRawPosition() throw();  
    //! Odometry interface method implementation.
    virtual PositionIDL getWaitRawPosition() throw (ETimeOut);
    //! Odometry interface method implementation.
    virtual VelocityIDL getVelocity() throw();    
    //! Odometry interface method implementation.
    virtual VelocityIDL getWaitVelocity() throw (ETimeOut);
    //! Odometry interface method implementation.
    virtual MotionStatusIDL getStatus() throw();    
    //! Odometry interface method implementation.
    virtual MotionStatusIDL getWaitStatus() throw (ETimeOut);

  protected:
    //-------------------------------------------------------------------------
    // protected methods
    //-------------------------------------------------------------------------
    
    //! None-thread safe set postion stuff.
    void protectedSetPosition(const PositionIDL& robot);
    //! Calculate sine and cosine of the origin heading.
    void setTransformation();

    //-------------------------------------------------------------------------
    // protected object data
    //-------------------------------------------------------------------------

    //! Supplier for events.
    StructuredPushSupplier * supplier_;

    //! Lock
    Mutex mutex_;
    //! Condition for getWaitPosition etc.
    Condition cond_;
    //! Current motion status.
    MotionStatusIDL status_;

    //! Flag to indicate the we dispatch events asynchrnously.
    bool asynchDispatching_;
    //! Thread for asynchronous dispatching of events.
    OdometryDispatcher dispatcherThread_;

    //! Origin of the coordinate frame.
    PositionIDL origin_;
    //! Raw position as reported by integrateData.
    PositionIDL position_;

    //! Catched sine of the origin heading.
    double sinHeading_;
    //! Catched cosine of the origin heading.
    double cosHeading_;

    //! Timeout for getWaitPosition etc.
    static ACE_Time_Value maxWait_;
  };

  //---------------------------------------------------------------------------
  // inline methods
  //---------------------------------------------------------------------------
  inline
  void
  OdometryImpl::setTransformation() {
    sinHeading_ = sin(origin_.heading);
    cosHeading_ = cos(origin_.heading);
  }
};
#endif // OdometryImpl_h
