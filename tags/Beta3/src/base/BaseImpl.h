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
#ifndef BaseImpl_h
#define BaseImpl_h

#include <ace/OS.h>

// #include <termios.h>

#include "idl/B21MotionS.h"
#include "miro/MotionImpl.h"

#include "BaseConnection.h"

// forward declerations
class ACE_Reactor;
namespace Miro
{
  class Exception;
  class CException;
};
namespace Mcp
{
  class EventHandler;
};

namespace Base
{
  // forward declerations
  class EventHandler;
  class Consumer;
  class Parameters;

  class Impl :  public virtual POA_Miro::B21Motion,
		public virtual Miro::MotionImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    Impl(ACE_Reactor * _reactor,
	 Miro::OdometryImpl * _odometry) throw(Miro::Exception);
    virtual ~Impl();

    // server behaviour
    void setStatusReportPeriod(unsigned int period);
    unsigned int getStatusReportPeriod();
    void setStatusReportFormat(unsigned int format);
    unsigned int getStatusReportFormat();

    //-------------------------------------------------------------------------
    // implementation of the idl interface
    //-------------------------------------------------------------------------

    // stop the base
    
    void limp() throw(Miro::EDevIO);
    void halt() throw(Miro::EDevIO);

    // set the speed
    void setVelocity(const Miro::VelocityIDL& vel)
      throw(Miro::EOutOfBounds, Miro::EDevIO);


    // Synchro Motion Interface
    void rotate(double velocity) throw(Miro::EDevIO, Miro::EOutOfBounds);
    void translate(CORBA::Long velocity) throw(Miro::EDevIO, Miro::EOutOfBounds);

    // Rotation Commands

    void rotateLimp() throw(Miro::EDevIO);
    void rotateHalt() throw(Miro::EDevIO);
    void waitRotateHalt() throw();

    void rotateLeft() throw(Miro::EDevIO);
    void rotateRight() throw(Miro::EDevIO);
    void rotateToPosition(CORBA::Double position)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void rotateRelative(CORBA::Double relative)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void rotateTorque(CORBA::Long relative)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void rotatePower(CORBA::Long relative)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    
    void setRotateVelocity(CORBA::Double velocity)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void setRotateAcceleration(CORBA::Double accel)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void setRotateTorque(CORBA::ULong torque)
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
    void setTranslateTorque(CORBA::ULong torque)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    double getDistortion() throw();

    // Queries

    double getWaitRotateCurrent() throw(Miro::ETimeOut, Miro::EDevIO);
    double getWaitTranslateCurrent() throw(Miro::ETimeOut, Miro::EDevIO);
    double getWaitBatteryVoltage() throw(Miro::ETimeOut, Miro::EDevIO);

    // protected:
    const Parameters& parameters;

    Consumer *          consumer;     // asynchronous processing of mcp output
    Mcp::EventHandler * mcpEventHandler;
    EventHandler *      eventHandler;
    Connection          connection;   // encapsulating communication to hardware

    static ACE_Time_Value maxWait;
  };
};

#endif




