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


#include "BaseDummyImpl.h"
#include "Parameters.h"

#include "miro/IO.h"

#include <iostream>

// #undef DEBUG

namespace Base
{
  using std::cout;
  using std::cerr;
  using std::endl;

  Dummy::Dummy():
    Miro::MotionImpl(Parameters::instance()->motion),
    rotateState(LIMP),
    rotateVelocity(), // 0x0080),
    rotateAcceleration(), // 0x0400),
    translateState(LIMP),
    translateVelocity(), // 0x800),
    translateAcceleration(), // 0x4000),
    stateMutex()
  {
#ifdef DEBUG
    cout << "Constructing BaseDummy" << endl;
#endif

    status.time.sec = 0;
    status.time.usec = 0;

    status.position.point.x = 0;
    status.position.point.y = 0;
    status.position.heading = 0.;

    status.velocity.translation = 0;
    status.velocity.rotation = 0.;

  }

  Dummy::~Dummy()
  {
#ifdef DEBUG
    cout << "Destructing BaseDummy" << endl;
#endif
  }

  // 
  // IDL interface implementation
  void 
  Dummy::limp() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "limp all motors" << endl;
#endif
  }

  void
  Dummy::halt() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "halt all motors" << endl;
#endif
  }

  Miro::MotionStatusIDL
  Dummy::getStatus() throw()
  {
#ifdef DEBUG
    cout << "get base status data" << endl;
#endif
 
    return status;
  }

  Miro::MotionStatusIDL
  Dummy::getWaitStatus() throw(Miro::ETimeOut)
  {
#ifdef DEBUG
    cout << "get base status data" << endl;
#endif
 
    return status;
  }

  // Synchro motion interface.
  void 
  Dummy::setRotation(double velocity) throw(Miro::EDevIO, Miro::EOutOfBounds)
  {
#ifdef DEBUG
    cout << "set rotational (" << velocity << ") velocity" << endl;
#endif

    status.velocity.rotation = velocity;
  }
   
  void 
  Dummy::setTranslation(CORBA::Long velocity) throw(Miro::EDevIO, Miro::EOutOfBounds)
  {
#ifdef DEBUG
    cout << "set translational (" << velocity << ") velocity" << endl;
#endif

    status.velocity.translation = velocity;
  }

  // Rotation Commands

  void 

  Dummy::rotateLimp() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "limp rotational motor" << endl;
#endif

    Miro::Guard guard(stateMutex);

    rotateState = LIMP;
  }

  void
  Dummy::rotateHalt() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "halt rotational motor" << endl;
#endif

    Miro::Guard guard(stateMutex);

    rotateState = HALT;
  }

  void
  Dummy::waitRotateHalt() throw()
  {
#ifdef DEBUG
    cout << "wait for halt of rotational motor" << endl;
#endif
  }

  void
  Dummy::rotateLeft() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "start rotating left" << endl;
#endif
  }

  void
  Dummy::rotateRight() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "start rotating right" << endl;
#endif
  }

  void
  Dummy::rotateToPosition(CORBA::Double position) 
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "rotate to angle (rad): " << position << endl;
#endif
    Miro::Guard guard(stateMutex);

    rotateState = TO_POSITION;
  }

  void
  Dummy::rotateRelative(CORBA::Double relative) throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "rotate by angle (rad): " << relative << endl;
#endif

    Miro::Guard guard(stateMutex);

    rotateState = TO_POSITION;
  }

  void
  Dummy::rotateTorque(CORBA::Long relative)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "rotate with torque: " << relative << endl;
#endif

    Miro::Guard guard(stateMutex);

    rotateState = TORQUE;
  }

  void
  Dummy::rotatePower(CORBA::Long relative)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "rotate with power: " << relative << endl;
#endif

    Miro::Guard guard(stateMutex);

    rotateState = POWER;
  }

  void
  Dummy::setRotateVelocity(CORBA::Double velocity)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "set rotate velocity (rad/s): " << velocity << endl;
#endif

    Miro::Guard guard(stateMutex);

    rotateVelocity = velocity;
  }

  void
  Dummy::setRotateAcceleration(CORBA::Double accel)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "set rotate acceleration (rad/s*s): " << accel << endl;
#endif

    Miro::Guard guard(stateMutex);

    rotateAcceleration = accel;
  }

  void
  Dummy::setRotateTorque(CORBA::ULong torque)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "set rotate torque: " << torque << endl;
#endif
  }

  //
  // Translation Commands
  //

  void
  Dummy::translateLimp() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "limp translational motor" << endl;
#endif

    Miro::Guard guard(stateMutex);

    translateState = LIMP;
  }

  void
  Dummy::translateHalt() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "halt translational motor" << endl;
#endif

    Miro::Guard guard(stateMutex);

    translateState = HALT;
  }

  void
  Dummy::waitTranslateHalt() throw()
  {
#ifdef DEBUG
    cout << "wait for halt of translational motor" << endl;
#endif
  }

  void
  Dummy::translateForward() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "start translating forward" << endl;
#endif
  }

  void
  Dummy::translateBackward() throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "start translating backward" << endl;
#endif
  }

  void
  Dummy::translateToPosition(CORBA::ULong position) throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "translate to position (mm): " << position << endl;
#endif

    Miro::Guard guard(stateMutex);

    translateState = TO_POSITION;
  }

  void
  Dummy::translateRelative(CORBA::Long relative) throw(Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "translate by distance (mm): " << relative << endl;
#endif

    Miro::Guard guard(stateMutex);

    translateState = TO_POSITION;
  }

  void
  Dummy::translateTorque(CORBA::Long relative)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "translate with torque: " << relative << endl;
#endif

    Miro::Guard guard(stateMutex);

    translateState = TORQUE;
  }

  void
  Dummy::translatePower(CORBA::Long relative) 
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "translate with power: " << relative << endl;
#endif

    Miro::Guard guard(stateMutex);

    translateState = POWER;
  }

  void
  Dummy::setTranslateVelocity(CORBA::ULong velocity)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "set translate velocity (mm/s): " << velocity << endl;
#endif

    Miro::Guard guard(stateMutex);

    translateVelocity = velocity;
  }

  void
  Dummy::setTranslateAcceleration(CORBA::ULong accel)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "set translate acceleration (mm/s*s): " << accel << endl;
#endif

    Miro::Guard guard(stateMutex);

    translateAcceleration = accel;
  }

  void
  Dummy::setTranslateTorque(CORBA::ULong torque)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "set translate torque: " << torque << endl;
#endif

    Miro::Guard guard(stateMutex);

    translateState = TORQUE;
  }


  //
  // querys
  //
  double
  Dummy::getWaitRotateCurrent() throw(Miro::ETimeOut, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "get the roatitional current" << endl;
#endif
    return 1.0;
  }

  double
  Dummy::getWaitTranslateCurrent() throw(Miro::ETimeOut, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "get the translational current" << endl;
#endif

    return 1.0;
  }

  double
  Dummy::getWaitBatteryVoltage() throw(Miro::ETimeOut, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "get the battery voltage" << endl;
#endif

    return 48.0;
  }

  void
  Dummy::setPosition(const Miro::PositionIDL& pos ) throw()
  {
#ifdef DEBUG
    cout << "set the position to: " << pos << endl;
#endif

    status.position = pos;
  }

  void
  Dummy::updatePosition(const Miro::PositionIDL& pos) throw()
  {
#ifdef DEBUG
    cout << "move the position by: " << pos << endl;
#endif

    status.position.point.x += pos.point.x;
    status.position.point.y += pos.point.y;
    status.position.heading += pos.heading;
  }

  Miro::PositionIDL
  Dummy::getRawPosition() throw()
  {
#ifdef DEBUG
    cout << "get the position" << endl;
#endif

    return status.position;
  }

  Miro::PositionIDL
  Dummy::getPosition() throw()
  {
#ifdef DEBUG
    cout << "get the position" << endl;
#endif

    return status.position;
  }

  Miro::PositionIDL
  Dummy::getWaitPosition() throw(Miro::ETimeOut)
  {
#ifdef DEBUG
    cout << "get the position" << endl;
#endif

    return status.position;
  }

  Miro::PositionIDL
  Dummy::getWaitRawPosition() throw(Miro::ETimeOut)
  {
#ifdef DEBUG
    cout << "get the raw position" << endl;
#endif

    return status.position;
  }

  void 
  Dummy::setVelocity(const Miro::VelocityIDL& vel) 
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
#ifdef DEBUG
    cout << "set rotational (" << vel. translation 
	 << ") and translational (" << vel.rotation << ") velocity" << endl;
#endif

    status.velocity = vel;
  }

  Miro::VelocityIDL
  Dummy::getVelocity()  throw()
  {
#ifdef DEBUG
    cout << "get the velocity" << endl;
#endif

    return status.velocity;
  }

  Miro::VelocityIDL
  Dummy::getWaitVelocity() throw(Miro::ETimeOut)
  {
#ifdef DEBUG
    cout << "get the velocity" << endl;
#endif

    return status.velocity;
  }


  // to be implemented soon!!!
  double 
  Dummy::getDistortion() throw()
  {
    return 0.;
  }

};
