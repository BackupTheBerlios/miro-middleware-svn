// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "OdometryImpl.h"

#include "TimeHelper.h"
#include "StructuredPushSupplier.h"

#include <cmath>

namespace Miro
{

  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value OdometryDispatcher::maxWait_(0, 100000);

  OdometryDispatcher::OdometryDispatcher(StructuredPushSupplier * _supplier,
					 bool _rawPositionEvents) :
    supplier_(_supplier),
    rawPositionEvents_(_rawPositionEvents),
    mutex_(),
    cond_(mutex_)
  {
    if (supplier_) {
      // Status Notify Event initialization
      notifyEvent_.header.fixed_header.event_type.domain_name = 
	CORBA::string_dup(supplier_->domainName().c_str());
      notifyEvent_.header.fixed_header.event_type.type_name = 
	CORBA::string_dup("Odometry");
      notifyEvent_.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent_.header.variable_header.length(0);   // put nothing here
      notifyEvent_.filterable_data.length(0);          // put nothing here

      if (rawPositionEvents_) {
	// Status Notify Raw Event initialization
	notifyRawEvent_.header.fixed_header.event_type.domain_name = 
	  CORBA::string_dup(supplier_->domainName().c_str());
	notifyRawEvent_.header.fixed_header.event_type.type_name = 
	  CORBA::string_dup("RawPosition");
	notifyRawEvent_.header.fixed_header.event_name = CORBA::string_dup("");
	notifyRawEvent_.header.variable_header.length(0);   // put nothing here
	notifyRawEvent_.filterable_data.length(0);          // put nothing here
      }
    }
  }

  void
  OdometryDispatcher::setData(const MotionStatusIDL& _status,
			const RawPositionIDL& _raw)
  {
    notifyEvent_.remainder_of_body <<= _status;
    notifyRawEvent_.remainder_of_body <<= _raw;
  };

  int
  OdometryDispatcher::svc()
  {
    while(!canceled()) {
      Guard guard(mutex_);
      ACE_Time_Value timeout(ACE_OS::gettimeofday());
      timeout += maxWait_;
      if (cond_.wait(&timeout) != -1 &&
	  !canceled()) {
	dispatch();
      }
    }

    return 0;
  }

  void
  OdometryDispatcher::dispatch()
  {
    supplier_->sendEvent(notifyEvent_);
    if (rawPositionEvents_) {
      supplier_->sendEvent(notifyRawEvent_);
    }
  }

  void
  OdometryDispatcher::cancel(bool _wait)
  {
    canceled_ = true;
    cond_.broadcast();
    Super::cancel(_wait);
  }


  /** Maximum wait time for cond_.wait calls. */
  ACE_Time_Value OdometryImpl::maxWait_(1, 0);

  /**
   * Implementation skeleton constructor.
   *
   * @param _supplier A pointer to the StructuredPushSupplier to use
   * for event emittion. If a NULL pointer is passed, the OdometryImpl
   * will not emit any events.
   *
   * @param _rawPositionEvents When set to true the OdometryImpl will
   * also emit RawPosition events.
   */
  OdometryImpl::OdometryImpl(StructuredPushSupplier * _supplier,
			     bool _rawPositionEvents,
			     bool _asychDispatching) :
    supplier_(_supplier),
    mutex_(),
    cond_(mutex_),
    asynchDispatching_(_asychDispatching),
    dispatcherThread_(_supplier, _rawPositionEvents),
    sinHeading_(0.0),
    cosHeading_(1.0)
  {
    position_.point.x = 0.;
    position_.point.y = 0.;
    position_.heading = 0.;

    origin_.point.x = 0.;
    origin_.point.y = 0.;
    origin_.heading = 0.;

    status_.time.sec = 0;
    status_.time.usec = 0;
    status_.position.point.x = 0.;
    status_.position.point.y = 0.;
    status_.position.heading = 0.;
    status_.velocity.translation = 0;
    status_.velocity.rotation = 0.;
  }
  
  // Implementation skeleton destructor
  OdometryImpl::~OdometryImpl()
  {
    dispatcherThread_.cancel();
  }
  
  /**
   * Integrates new odometry data from the low-level device layer
   * into the OdometryImpl.
   *
   * @param data The new odometry position. The data provided is in
   * the raw coordinate frame. That is the origin of the coordinate frame
   * is arbitary, only the frame has to be eucledian.
   */
  void 
  OdometryImpl::integrateData(const MotionStatusIDL& data)
  {
    assert(data.position.heading > -M_PI &&
	   data.position.heading <= M_PI);

    { // scope for guard
      Guard guard(mutex_);
      
      // set all the data
      
      status_.time = data.time;
      status_.velocity = data.velocity;
    
      // save robots own position for coordinate frame adjustions
      position_ = data.position;
    
      // get position from status report
      // compute new world position of robot
      double x = data.position.point.x - origin_.point.x;
      double y = data.position.point.y - origin_.point.y;
    
      // rotate position by heading of origin 
      status_.position.point.x = x * cosHeading_ - y * sinHeading_;
      status_.position.point.y = x * sinHeading_ + y * cosHeading_;
      status_.position.heading = data.position.heading + origin_.heading;

      // normalize data
      if (status_.position.heading <= -M_PI)
	status_.position.heading += 2 * M_PI;
      else if (status_.position.heading > M_PI)
	status_.position.heading -= 2 * M_PI;

      cond_.broadcast();
    }

    // send events
    if (supplier_) {
      RawPositionIDL raw;
      raw.time = data.time;
      raw.position = data.position;

      if (asynchDispatching_) {
	Guard guard(dispatcherThread_.mutex_);
	dispatcherThread_.setData(status_, raw);
	dispatcherThread_.cond_.broadcast();
      }
      else {
	dispatcherThread_.setData(status_, raw);
	dispatcherThread_.dispatch();
      }
    }
  }

  PositionIDL OdometryImpl::getPosition() throw()
  {
    Guard guard(mutex_);
    return status_.position;
  }
  
  PositionIDL
  OdometryImpl::getWaitPosition() throw (ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw ETimeOut();

    return status_.position;
  }
  
  PositionIDL
  OdometryImpl::getRawPosition() throw()
  {
    Guard guard(mutex_);
    return position_;
  }

  PositionIDL
  OdometryImpl::getWaitRawPosition() throw (ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw ETimeOut();

    return position_;
  }
  
  void
  OdometryImpl::setPosition(const PositionIDL& pos) throw()
  {
    Guard guard(mutex_);
    protectedSetPosition(pos);
  }
  
  void
  OdometryImpl::updatePosition(const Miro::PositionIDL& dPos) throw()
  {
    PositionIDL robot;
    Guard guard(mutex_);
    robot.point.x = status_.position.point.x + dPos.point.x;
    robot.point.y = status_.position.point.y + dPos.point.y;
    robot.heading = status_.position.heading + dPos.heading;

    protectedSetPosition(robot);
  }
  
  VelocityIDL 
  OdometryImpl::getVelocity() throw()
  {
    Guard guard(mutex_);
    return status_.velocity;
  }
  
  VelocityIDL
  OdometryImpl::getWaitVelocity() throw (ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw ETimeOut();

    return status_.velocity;
  }
  
  MotionStatusIDL
  OdometryImpl::getStatus() throw()
  {
    Guard guard(mutex_);
    return status_;
  }
  
  MotionStatusIDL
  OdometryImpl::getWaitStatus() throw (ETimeOut)
  {
    Guard guard(mutex_);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());

    timeout += maxWait_;
    if (cond_.wait(&timeout) == -1)
      throw ETimeOut();
    return status_;
  }

  void
  OdometryImpl::protectedSetPosition(const PositionIDL& robot)
  {
    // calculate heading
    origin_.heading = robot.heading - position_.heading;
    setTransformation();

    // calculate origin

    // commanded position of the robot
    double xR = robot.point.x;
    double yR = robot.point.y;

    // origin matching the current base position to 
    // the requested robot position
    origin_.point.x = position_.point.x - (xR * cosHeading_ + yR * sinHeading_);
    origin_.point.y = position_.point.y - (-xR * sinHeading_ + yR * cosHeading_);
  }
};
