// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef motionImpl_hh
#define motionImpl_hh

#include "idl/MotionS.h"
#include "idl/OdometryC.h"
#include "miro/DifferentialMotionImpl.h"
#include "miro/Exception.h"

// forward declerations
class ACE_Reactor;
namespace Miro
{
  class Exception;
};

namespace Pioneer
{
  // forward declerations
  class Connection;
  class Consumer;
  class Parameters;

  class MotionImpl :  public virtual POA_Miro::Motion,
		      public virtual ::Miro::DifferentialMotionImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    MotionImpl(Connection& _connection,
	       Miro::Odometry_ptr _pOdometry =  Miro::Odometry::_nil())
      throw(Miro::Exception);
    virtual ~MotionImpl();

    //-------------------------------------------------------------------------
    // General motion interface
    //-------------------------------------------------------------------------
    virtual void limp() 
      throw(Miro::EDevIO);
    virtual void setVelocity (const Miro::VelocityIDL & vel)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    //-------------------------------------------------------------------------
    // Differential motion interface
    //-------------------------------------------------------------------------
    virtual void setLRVelocity(CORBA::Long left, CORBA::Long right) 
      throw(Miro::EDevIO);

    //-------------------------------------------------------------------------
    // Closed motion interface
    //-------------------------------------------------------------------------
    virtual void rotateToPosition(CORBA::Double heading) 
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual void rotateRelative(CORBA::Double angle)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual void translateRelative(CORBA::Long distance)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

    // protected:
    Connection&         connection; // encapsulating communication to hardware
    Miro::Odometry_var  pOdometry;  // odometry for absulute turning
    const Parameters *  params_;    // parameter struct

    static ACE_Time_Value maxWait;
  };
};

#endif




