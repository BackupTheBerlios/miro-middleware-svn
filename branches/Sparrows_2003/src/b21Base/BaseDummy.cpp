
#include "MiroRCSID.h"

#include "BaseDummy.h"
#include "MiroPosition.h"

#include <iostream>

// #undef DEBUG

namespace Base
{
  Dummy::Dummy():
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

  Miro::MotionStatusDataIDL
  Dummy::getStatus()
  {
#ifdef DEBUG
    cout << "get base status data" << endl;
#endif
 
    return status;
  }

  Miro::MotionStatusDataIDL
  Dummy::getWaitStatus() throw(Miro::ETimeOut)
  {
#ifdef DEBUG
    cout << "get base status data" << endl;
#endif
 
    return status;
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
  Dummy::waitRotateHalt()
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
  Dummy::waitTranslateHalt()
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
  Dummy::setPosition(const Miro::PositionIDL& pos )
  {
#ifdef DEBUG
    cout << "set the position to: " << Miro::Position(pos) << endl;
#endif

    status.position = pos;
  }

  void
  Dummy::updatePosition(const Miro::PositionIDL& pos)
  {
#ifdef DEBUG
    cout << "move the position by: " << Miro::Position(pos) << endl;
#endif

    status.position.point.x += pos.point.x;
    status.position.point.y += pos.point.y;
    status.position.heading += pos.heading;
  }

  Miro::PositionDataIDL
  Dummy::getPosition()
  {
#ifdef DEBUG
    cout << "get the position" << endl;
#endif

    Miro::PositionDataIDL data;

    data.time = status.time;
    data.pos = status.position;

    return data;
  }

  Miro::PositionDataIDL
  Dummy::getWaitPosition() throw(Miro::ETimeOut)
  {
#ifdef DEBUG
    cout << "get the position" << endl;
#endif

    Miro::PositionDataIDL data;

    data.time = status.time;
    data.pos = status.position;

    return data;
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

  Miro::VelocityDataIDL
  Dummy::getVelocity() 
  {
#ifdef DEBUG
    cout << "get the velocity" << endl;
#endif

    Miro::VelocityDataIDL data;

    data.time = status.time;
    data.velocity = status.velocity;

    return data;
  }

  Miro::VelocityDataIDL
  Dummy::getWaitVelocity() throw(Miro::ETimeOut)
  {
#ifdef DEBUG
    cout << "get the velocity" << endl;
#endif

    Miro::VelocityDataIDL data;

    data.time = status.time;
    data.velocity = status.velocity;

    return data;
  }
};
