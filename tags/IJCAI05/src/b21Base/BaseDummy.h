#ifndef BaseDummy_h
#define BaseDummy_h

#include "MiroBaseS.h"
#include "MiroSynch.h"

#include "BaseDevice.h"

namespace Base
{
  // forward declarations
  class Dummy :  public virtual POA_Miro::Base
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    Dummy();
    virtual ~Dummy();

    //-------------------------------------------------------------------------
    // implementation of the idl interface
    //-------------------------------------------------------------------------

    // stop the base
    
    void limp() throw(Miro::EDevIO);
    void halt() throw(Miro::EDevIO);

    // set the speed
    void setVelocity(const Miro::VelocityIDL& vel)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    Miro::VelocityDataIDL getVelocity();
    Miro::VelocityDataIDL getWaitVelocity() throw(Miro::ETimeOut);

    Miro::MotionStatusDataIDL getStatus();
    Miro::MotionStatusDataIDL getWaitStatus() throw(Miro::ETimeOut);

    // tell robot to see his current position as
    // the given coordinates and heading

    void setPosition(const Miro::PositionIDL& pos );
    void updatePosition( const Miro::PositionIDL& dPos);
    Miro::PositionDataIDL getPosition();
    Miro::PositionDataIDL getWaitPosition() throw(Miro::ETimeOut);

    // Rotation Commands

    void rotateLimp() throw(Miro::EDevIO);
    void rotateHalt() throw(Miro::EDevIO);
    void waitRotateHalt();

    void rotateLeft() throw(Miro::EDevIO);
    void rotateRight() throw(Miro::EDevIO);
    void rotateToPosition(CORBA::Double position)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void rotateRelative(CORBA::Double relative) throw(Miro::EDevIO);
    void rotateTorque(CORBA::Long relative)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void rotatePower(CORBA::Long relative)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    
    void setRotateVelocity(CORBA::Double velocity)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void setRotateAcceleration(CORBA::Double accel)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void setRotateTorque(CORBA::ULong tork)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    // Translation Commands
    
    void translateLimp() throw(Miro::EDevIO);
    void translateHalt() throw(Miro::EDevIO);
    void waitTranslateHalt();

    void translateForward() throw(Miro::EDevIO);
    void translateBackward() throw(Miro::EDevIO);
    void translateToPosition(CORBA::ULong position) throw(Miro::EDevIO);
    void translateRelative(CORBA::Long relative) throw(Miro::EDevIO);
    void translateTorque(CORBA::Long relative)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void translatePower(CORBA::Long relative)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    void setTranslateVelocity(CORBA::ULong velocity) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void setTranslateAcceleration(CORBA::ULong accel)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void setTranslateTorque(CORBA::ULong tork) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    // Queries

    double getWaitRotateCurrent() throw(Miro::ETimeOut, Miro::EDevIO);
    double getWaitTranslateCurrent() throw(Miro::ETimeOut, Miro::EDevIO);
    double getWaitBatteryVoltage() throw(Miro::ETimeOut, Miro::EDevIO);

  protected:
    Miro::MotionStatusDataIDL status;


    // the internal state of the base
    // the defaults are given as comment
    // these are the defaults of the hardware on reset
    // as documented in the B21 users Guide

    StateType rotateState;
    
    double rotateVelocity;  // 0x0080
    double rotateAcceleration; // 0x0400
 
    StateType translateState;
    
    double translateVelocity;     // 0x800 Default 100 mm/s
    double translateAcceleration; // Default 50 mm/s 0x4000

    // we have to make access to it threadsafe

    Miro::Mutex stateMutex;
  };
};

#endif




