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
#ifndef SparrowMotionImpl_h
#define SparrowMotionImpl_h

#include "miro/SparrowMotionS.h"
#include "miro/DifferentialMotionImpl.h"
#include "miro/Exception.h"


// forward declerations
class ACE_Reactor;

namespace Sparrow
{
  // forward declerations
  class Connection;
  class Parameters;

  class MotionImpl :  public virtual POA_Miro::SparrowMotion,
		      public virtual Miro::DifferentialMotionImpl
  {
    //-------------------------------------------------------------------------
    // public methods
    //-------------------------------------------------------------------------
  public:
    // Constructor/Destructor
    MotionImpl(Connection& _connection) throw(Miro::Exception);
    virtual ~MotionImpl();

    //-------------------------------------------------------------------------
    // Motion interface
    //-------------------------------------------------------------------------
    void limp() throw(Miro::EDevIO);
    void setVelocity(const Miro::VelocityIDL& vel)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void setLRVelocity(CORBA::Long left, CORBA::Long right)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void setLRPower(CORBA::Long left, CORBA::Long right)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void initMax(short maxAcc, short maxPower, short maxSpeed, double maxTurn)
      throw();

    // protected:
    const Parameters *  params_;
    Connection&         connection; // encapsulating communication to hardware
    
  };
};

#endif




