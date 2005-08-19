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
#ifndef BaseDummyImpl_h
#define BaseDummyImpl_h

#include "idl/B21MotionS.h"
#include "miro/MotionImpl.h"
#include "miro/Synch.h"

#include "BaseDevice.h"

namespace Base
{
  // forward declarations
  class Dummy :  public virtual POA_Miro::B21Motion,
		 public virtual Miro::MotionImpl
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
    Miro::VelocityIDL getVelocity() throw();
    Miro::VelocityIDL getWaitVelocity() throw(Miro::ETimeOut);

    Miro::MotionStatusIDL getStatus() throw();
    Miro::MotionStatusIDL getWaitStatus() throw(Miro::ETimeOut);

    // tell robot to see his current position as
    // the given coordinates and heading

    void setPosition(const Miro::PositionIDL& pos ) throw();
    void updatePosition( const Miro::PositionIDL& dPos) throw();
    Miro::PositionIDL getPosition() throw();
    Miro::PositionIDL getWaitPosition() throw(Miro::ETimeOut);
    Miro::PositionIDL getRawPosition() throw();  
    Miro::PositionIDL getWaitRawPosition() throw(Miro::ETimeOut);

    // Synchro Motion Interface
    void setRotation(double velocity) throw(Miro::EDevIO, Miro::EOutOfBounds);
    void setTranslation(CORBA::Long velocity) throw(Miro::EDevIO, Miro::EOutOfBounds);

    // Rotation Commands

    void rotateLimp() throw(Miro::EDevIO);
    void rotateHalt() throw(Miro::EDevIO);
    void waitRotateHalt() throw();

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
    void waitTranslateHalt() throw();

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

    double getDistortion() throw();

    // Queries

    double getWaitRotateCurrent() throw(Miro::ETimeOut, Miro::EDevIO);
    double getWaitTranslateCurrent() throw(Miro::ETimeOut, Miro::EDevIO);
    double getWaitBatteryVoltage() throw(Miro::ETimeOut, Miro::EDevIO);

  protected:
    Miro::MotionStatusIDL status;


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




