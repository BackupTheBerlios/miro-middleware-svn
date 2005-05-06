
#include "MiroRCSID.h"

#include "BaseDummy.h"
#include "MiroPosition.h"
#include "miro/Log.h"

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
    MIRO_DBG( B21 , LL_CTOR_DTOR , "Constructing BaseDummy" );

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
    MIRO_DBG( B21 , LL_CTOR_DTOR , "Destructing BaseDummy" );

  }

  // 
  // IDL interface implementation
  void 
  Dummy::limp() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "limp all motors" );
  }

  void
  Dummy::halt() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "halt all motors" );
  }

  Miro::MotionStatusDataIDL
  Dummy::getStatus()
  {
    MIRO_DBG( B21 , LL_PRATTLE , "get base status data" );
 
    return status;
  }

  Miro::MotionStatusDataIDL
  Dummy::getWaitStatus() throw(Miro::ETimeOut)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "get base status data" );
 
    return status;
  }


  // Rotation Commands

  void 

  Dummy::rotateLimp() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "limp rotational motor" );

    Miro::Guard guard(stateMutex);

    rotateState = LIMP;
  }

  void
  Dummy::rotateHalt() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "halt rotational motor" );

    Miro::Guard guard(stateMutex);

    rotateState = HALT;
  }

  void
  Dummy::waitRotateHalt()
  {
    MIRO_DBG( B21 , LL_PRATTLE , "wait for halt of rotational motor" );
  }

  void
  Dummy::rotateLeft() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "start rotating left" );
  }

  void
  Dummy::rotateRight() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "start rotating right" );
  }

  void
  Dummy::rotateToPosition(CORBA::Double position) 
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE ,  "rotate to angle (rad): " << position );

    Miro::Guard guard(stateMutex);

    rotateState = TO_POSITION;
  }

  void
  Dummy::rotateRelative(CORBA::Double relative) throw(Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE ,  "rotate by angle (rad): " << relative );

    Miro::Guard guard(stateMutex);

    rotateState = TO_POSITION;
  }

  void
  Dummy::rotateTorque(CORBA::Long relative)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE ,  "rotate with torque: " << relative );

    Miro::Guard guard(stateMutex);

    rotateState = TORQUE;
  }

  void
  Dummy::rotatePower(CORBA::Long relative)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE ,  "rotate with power: " << relative );

    Miro::Guard guard(stateMutex);

    rotateState = POWER;
  }

  void
  Dummy::setRotateVelocity(CORBA::Double velocity)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE ,  "set rotate velocity (rad/s): " << velocity );

    Miro::Guard guard(stateMutex);

    rotateVelocity = velocity;
  }

  void
  Dummy::setRotateAcceleration(CORBA::Double accel)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE ,  "set rotate acceleration (rad/s*s): " << accel );

    Miro::Guard guard(stateMutex);

    rotateAcceleration = accel;
  }

  void
  Dummy::setRotateTorque(CORBA::ULong torque)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE ,  "set rotate torque: " << torque );
  }

  //
  // Translation Commands
  //

  void
  Dummy::translateLimp() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "limp translational motor" );

    Miro::Guard guard(stateMutex);

    translateState = LIMP;
  }

  void
  Dummy::translateHalt() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "halt translational motor" );

    Miro::Guard guard(stateMutex);

    translateState = HALT;
  }

  void
  Dummy::waitTranslateHalt()
  {
    MIRO_DBG( B21 , LL_PRATTLE , "wait for halt of translational motor" );
  }

  void
  Dummy::translateForward() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "start translating forward" );
  }

  void
  Dummy::translateBackward() throw(Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "start translating backward" );
  }

  void
  Dummy::translateToPosition(CORBA::ULong position) throw(Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "translate to position (mm): " << position );

    Miro::Guard guard(stateMutex);

    translateState = TO_POSITION;
  }

  void
  Dummy::translateRelative(CORBA::Long relative) throw(Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "translate by distance (mm): " << relative );

    Miro::Guard guard(stateMutex);

    translateState = TO_POSITION;
  }

  void
  Dummy::translateTorque(CORBA::Long relative)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "translate with torque: " << relative );

    Miro::Guard guard(stateMutex);

    translateState = TORQUE;
  }

  void
  Dummy::translatePower(CORBA::Long relative) 
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "translate with power: " << relative );

    Miro::Guard guard(stateMutex);

    translateState = POWER;
  }

  void
  Dummy::setTranslateVelocity(CORBA::ULong velocity)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "set translate velocity (mm/s): " << velocity );

    Miro::Guard guard(stateMutex);

    translateVelocity = velocity;
  }

  void
  Dummy::setTranslateAcceleration(CORBA::ULong accel)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "set translate acceleration (mm/s*s): " << accel );

    Miro::Guard guard(stateMutex);

    translateAcceleration = accel;
  }

  void
  Dummy::setTranslateTorque(CORBA::ULong torque)
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "set translate torque: " << torque );

    Miro::Guard guard(stateMutex);

    translateState = TORQUE;
  }


  //
  // querys
  //
  double
  Dummy::getWaitRotateCurrent() throw(Miro::ETimeOut, Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "get the roatitional current" );

    return 1.0;
  }

  double
  Dummy::getWaitTranslateCurrent() throw(Miro::ETimeOut, Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "get the translational current" );

    return 1.0;
  }

  double
  Dummy::getWaitBatteryVoltage() throw(Miro::ETimeOut, Miro::EDevIO)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "get the battery voltage" );

    return 48.0;
  }

  void
  Dummy::setPosition(const Miro::PositionIDL& pos )
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "set the position to: " << Miro::Position(pos) );

    status.position = pos;
  }

  void
  Dummy::updatePosition(const Miro::PositionIDL& pos)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "move the position by: " << Miro::Position(pos) );

    status.position.point.x += pos.point.x;
    status.position.point.y += pos.point.y;
    status.position.heading += pos.heading;
  }

  Miro::PositionDataIDL
  Dummy::getPosition()
  {
    MIRO_DBG( B21 , LL_PRATTLE , "get the position" );

    Miro::PositionDataIDL data;

    data.time = status.time;
    data.pos = status.position;

    return data;
  }

  Miro::PositionDataIDL
  Dummy::getWaitPosition() throw(Miro::ETimeOut)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "get the position" );

    Miro::PositionDataIDL data;

    data.time = status.time;
    data.pos = status.position;

    return data;
  }

  void 
  Dummy::setVelocity(const Miro::VelocityIDL& vel) 
    throw(Miro::EOutOfBounds, Miro::EDevIO)
  {
    MIRO_DBG_OSTR( B21 , LL_PRATTLE , "set rotational (" << vel. translation 
                                   << ") and translational (" << vel.rotation << ") velocity" );

    status.velocity = vel;
  }

  Miro::VelocityDataIDL
  Dummy::getVelocity() 
  {
    MIRO_DBG( B21 , LL_PRATTLE , "get the velocity" );

    Miro::VelocityDataIDL data;

    data.time = status.time;
    data.velocity = status.velocity;

    return data;
  }

  Miro::VelocityDataIDL
  Dummy::getWaitVelocity() throw(Miro::ETimeOut)
  {
    MIRO_DBG( B21 , LL_PRATTLE , "get the velocity" );

    Miro::VelocityDataIDL data;

    data.time = status.time;
    data.velocity = status.velocity;

    return data;
  }
};
