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
#ifndef GripperImpl_hh
#define GripperImpl_hh

#include "miro/GripperS.h"

#include "miro/Exception.h"

// forward declarations
class ACE_Reactor;

namespace Pioneer
{
  // forward declarations
  class Connection;
  class Consumer;
};

namespace Miro
{
  class Exception;

  class GripperImpl :  public virtual POA_Miro::Gripper
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    GripperImpl(Pioneer::Connection& _connection,
		    Pioneer::Consumer& _consumer) throw(Miro::Exception);
    virtual ~GripperImpl();

    virtual void setGripPressure(short value)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    virtual void openGrip()
      throw(Miro::EDevIO);
    virtual void closeGrip()
      throw(Miro::EDevIO);
    virtual void raiseGrip()
      throw(Miro::EDevIO);
    virtual void lowerGrip()
      throw(Miro::EDevIO);
    virtual void storeGrip()
      throw(Miro::EDevIO);
    virtual void deployGrip()
      throw(Miro::EDevIO);
    virtual void stopGrip()
      throw(Miro::EDevIO);
    virtual void moveGrip(short ms)
      throw(Miro::EOutOfBounds, Miro::EDevIO);

  protected:
    Pioneer::Connection&         connection; // encapsulating communication to hardware
    Pioneer::Consumer&           consumer;   // asynchronous processing of pioneer output
    
    static ACE_Time_Value maxWait;
  };

};

#endif




