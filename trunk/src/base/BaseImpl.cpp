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


#include "miro/Synch.h"
#include "miro/Exception.h"

#include "mcp/McpEventHandler.h"

#include "BaseEventHandler.h"
#include "BaseConsumer.h"
#include "BaseImpl.h"
#include "Parameters.h"

#include <iostream>
#include <cmath>

// #include <termios.h>

// #undef DEBUG

using Miro::EDevIO;
using Miro::ETimeOut;
using Miro::EOutOfBounds;

/***************************************************************************
*
*  Base Implementation 
*
*  Handles output from base
*
***************************************************************************/

namespace Base
{
  ACE_Time_Value Impl::maxWait;


  Impl::Impl(ACE_Reactor * _reactor, 
	     Miro::OdometryImpl * _odometry) 
    throw(Miro::Exception) :
    Miro::MotionImpl(Parameters::instance()->motion),
    parameters(*Parameters::instance()),
    consumer(new Consumer(connection, _odometry)),
    mcpEventHandler(new Mcp::EventHandler(connection, consumer, parameters)),
    eventHandler(new EventHandler(connection, *consumer, parameters)),
    connection(_reactor, mcpEventHandler, consumer, eventHandler, parameters)
  {
    // maximum wait time for cond.wait calls
    maxWait = ACE_Time_Value(0, 500000);

    // initialize the internal position
    // management of the base hardware
    // set the robot into the middle, so we have 
    // 650m to drive in each direction befor overflow
    // of the counters - I don´t think the batteries will do it that long
    //  connection.loadHeading(0);
    //  connection.loadPosition(INITIAL_XPOS, INITIAL_YPOS);

    // we should initialize server side position management
    // still work to do!

    // position.setMcpPosition(0x8000, 0x8000);
    // position.setWorldPosition(0.0, 0.0);

    // set up status reports from base
    setStatusReportFormat(0);   // report required values
    setStatusReportPeriod(DEFAULT_REPORT_PERIOD);
  }

  Impl::~Impl()
  {
#ifdef DEBUG
    cout << "Destructing BaseImpl" << endl;
#endif
  }

  // 
  // IDL interface implementation
  void 
  Impl::limp() throw(EDevIO)
  {
    Miro::VelocityIDL v;
    v.translation = 0;
    v.rotation = 0.;

    Miro::Guard guard(mutex_);
    setTargetVelocity(v);
    connection.limp();
  }

  void
  Impl::halt() throw(EDevIO)
  {
    Miro::VelocityIDL v;
    v.translation = 0;
    v.rotation = 0.;

    Miro::Guard guard(mutex_);
    setTargetVelocity(v);
    connection.halt();
  }

  // Synchro Motion Interface
  void 
  Impl::rotate(double velocity) throw(Miro::EDevIO, Miro::EOutOfBounds)
  {
    if (fabs(velocity) > parameters.motion.maxRotation)
      throw EOutOfBounds();

    connection.setRotateVelocity(abs(rad2base(velocity)));

    if (velocity > 0.0) {
      connection.rotateVelocityNeg();
    }
    else if (velocity < 0.0) {
      connection.rotateVelocityPos();
    }
    else {
      connection.rotateHalt();
    }
  }

  void
  Impl::translate(CORBA::Long velocity) throw(Miro::EDevIO, Miro::EOutOfBounds)
  {
    if (abs(velocity) > parameters.motion.maxTranslation)
      throw EOutOfBounds();

    connection.setTranslateVelocity(abs((int)(velocity)));

    if (velocity < 0.0) {
      connection.translateVelocityNeg();
    }
    else if (velocity > 0.0) {
      connection.translateVelocityPos();
    }
    else {
      connection.translateHalt();
    }
  }

  // Rotation Commands

  void 
  Impl::rotateLimp() throw(EDevIO)
  {
    Miro::Guard guard(mutex_);
    targetVelocity_.rotation = 0.;
    connection.rotateLimp();
  }

  void
  Impl::rotateHalt() throw(EDevIO)
  {
    Miro::Guard guard(mutex_);
    targetVelocity_.rotation = 0.;
    connection.rotateHalt();
  }

  void
  Impl::waitRotateHalt() throw()
  {
    Miro::Guard guard(consumer->rotateHaltMutex);

    if (connection.rotateState <= HALT)
      return;
    consumer->rotateHaltCond.wait();
  }

  void
  Impl::rotateLeft() throw(EDevIO)
  {
    connection.rotateVelocityNeg();
  }

  void
  Impl::rotateRight() throw(EDevIO)
  {
    connection.rotateVelocityPos();
  }

  void
  Impl::rotateToPosition(CORBA::Double position) throw(EOutOfBounds, EDevIO)
  {
    if (position <= -M_PI || M_PI < position)
      throw EOutOfBounds();

    connection.rotateToPosition(rad2base(position));
  }

  void
  Impl::rotateRelative(CORBA::Double relative) throw(EOutOfBounds, EDevIO)
  {
    if (relative <= -(M_PI * 2) || (M_PI * 2) <= relative)
      throw EOutOfBounds();

    connection.rotateRelative(rad2base(relative));
  }

  void
  Impl::rotateTorque(CORBA::Long relative) throw(EOutOfBounds, EDevIO)
  {
    connection.rotateTorque(relative);
  }

  void
  Impl::rotatePower(CORBA::Long relative) throw(EOutOfBounds, EDevIO)
  {
    connection.rotatePower(relative);
  }

  void
  Impl::setRotateVelocity(CORBA::Double velocity) throw(EOutOfBounds, EDevIO)
  {
    if (velocity < 0. || velocity > parameters.motion.maxRotation)
      throw EOutOfBounds();

    connection.setRotateVelocity(rad2base(velocity));
  }

  void
  Impl::setRotateAcceleration(CORBA::Double accel) throw(EOutOfBounds, EDevIO)
  {
    if (accel < 0. || accel > parameters.maxRotAccel)
      throw EOutOfBounds();

    connection.setRotateAcceleration(rad2base(accel));
  }

  void
  Impl::setRotateTorque(CORBA::ULong torque) throw(EOutOfBounds, EDevIO)
  {
    connection.setRotateTorque(torque);
  }

  //
  // Translation Commands
  //

  void
  Impl::translateLimp() throw(EDevIO)
  {
    Miro::Guard guard(mutex_);
    targetVelocity_.translation = 0;
    connection.translateLimp();
  }

  void
  Impl::translateHalt() throw(EDevIO)
  {
    Miro::Guard guard(mutex_);
    targetVelocity_.translation = 0;
    connection.translateHalt();
  }

  void
  Impl::waitTranslateHalt() throw()
  {
    Miro::Guard guard(consumer->translateHaltMutex);

    if (connection.translateState <= HALT)
      return;
    consumer->translateHaltCond.wait();
  }

  void
  Impl::translateForward() throw(EDevIO)
  {
    connection.translateVelocityPos();
  }

  void
  Impl::translateBackward() throw(EDevIO)
  {
    connection.translateVelocityNeg();
  }

  void
  Impl::translateToPosition(CORBA::ULong position) throw(EDevIO)
  {
    connection.translateToPosition(position);
  }

  void
  Impl::translateRelative(CORBA::Long relative) throw(EDevIO)
  {
    connection.translateRelative(relative);
  }

  void
  Impl::translateTorque(CORBA::Long relative) throw(EOutOfBounds, EDevIO)
  {
    connection.translateTorque(relative);
  }

  void
  Impl::translatePower(CORBA::Long relative) throw(EOutOfBounds, EDevIO)
  {
    connection.translatePower(relative);
  }

  void
  Impl::setTranslateVelocity(CORBA::ULong velocity) throw(EOutOfBounds, EDevIO)
  {
    if ((long)velocity > parameters.motion.maxTranslation)
      throw EOutOfBounds();

    connection.setTranslateVelocity(velocity);
  }

  void
  Impl::setTranslateAcceleration(CORBA::ULong accel) throw(EOutOfBounds, EDevIO)
  {
    if ((int)accel > parameters.maxTransAccel)
      throw EOutOfBounds();

    connection.setTranslateAcceleration(accel);
  }

  void
  Impl::setTranslateTorque(CORBA::ULong torque) throw(EOutOfBounds, EDevIO)
  {
    connection.setTranslateTorque(torque);
  }


  //
  // querys
  //
  double
  Impl::getWaitRotateCurrent() throw(ETimeOut, EDevIO)
  {
    Miro::Guard guard(consumer->rotateCurrentMutex);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait;

    connection.rotateCurrent();
    if (consumer->rotateCurrentCond.wait(&timeout) == -1)
      throw ETimeOut();

    return ((double)consumer->rotateCurrent) / 10.0;
  }

  double
  Impl::getWaitTranslateCurrent() throw(ETimeOut, EDevIO)
  {
    Miro::Guard guard(consumer->translateCurrentMutex);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait;

    connection.translateCurrent();
    if (consumer->translateCurrentCond.wait(&timeout) == -1)
      throw ETimeOut();

    return ((double)consumer->translateCurrent) / 10.0;
  }

  double
  Impl::getWaitBatteryVoltage() throw(ETimeOut, EDevIO)
  {
    Miro::Guard guard(consumer->batteryVoltageMutex);
    ACE_Time_Value timeout(ACE_OS::gettimeofday());
    timeout += maxWait;

    connection.batteryVoltage();
    if (consumer->batteryVoltageCond.wait(&timeout) == -1)
      throw ETimeOut();

    return consumer->batteryVoltage;
  }

  void 
  Impl::setVelocity(const Miro::VelocityIDL& vel) throw(EOutOfBounds, EDevIO)
  {
    testVelocityBounds(vel);

    Miro::Guard guard(mutex_);
    setTargetVelocity(vel);

    connection.setRotateVelocity(abs(rad2base(vel.rotation)));

    if (vel.rotation > 0.0) {
      connection.rotateVelocityNeg();
    }
    else if (vel.rotation < 0.0) {
      connection.rotateVelocityPos();
    }
    else {
      connection.rotateHalt();
    }
  
    connection.setTranslateVelocity(abs((int)(vel.translation)));

    if (vel.translation < 0.0) {
      connection.translateVelocityNeg();
    }
    else if (vel.translation > 0.0) {
      connection.translateVelocityPos();
    }
    else {
      connection.translateHalt();
    }
  }

  // to be implemented soon!!!
  double 
  Impl::getDistortion() throw()
  {
    return 0.;
  }

  // Server Behaviour

  void
  Impl::setStatusReportPeriod(unsigned int period)
  {
    connection.setStatusReportPeriod(period); 
  }

  unsigned int 
  Impl::getStatusReportPeriod() 
  {
    return connection.getStatusReportPeriod(); 
  }

  void 
  Impl::setStatusReportFormat(unsigned int format) 
  { 
    connection.setStatusReportFormat(format | REQUIRED_STATUS_DATA); 
  }

  unsigned int 
  Impl::getStatusReportFormat() 
  { 
    return connection.getStatusReportFormat(); 
  }
};
