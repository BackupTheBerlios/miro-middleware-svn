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

#include "miro/MotionS.h"
#include "miro/MotionImpl.h"
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

  class MotionImpl :  public virtual POA_Miro::Motion,
		      public virtual ::Miro::MotionImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    MotionImpl(Connection& _connection,
	       Consumer& _consumer) throw(Miro::Exception);
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
    Consumer&           consumer;   // asynchronous processing of pioneer output

    static ACE_Time_Value maxWait;
  };
};

#endif




