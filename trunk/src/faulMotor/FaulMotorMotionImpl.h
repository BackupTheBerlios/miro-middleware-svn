// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef faulMotionImpl_hh
#define faulMotionImpl_hh

#include "idl/SparrowMotionS.h"
#include "idl/OdometryC.h"
#include "miro/DifferentialMotionImpl.h"
#include "miro/Exception.h"

// forward declerations
class ACE_Reactor;
namespace Miro
{
  class Exception;
};

namespace FaulMotor
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

    void setLRPower(CORBA::Long, CORBA::Long)
      throw(Miro::EOutOfBounds, Miro::EDevIO);
    void initMax(short maxAcc, short maxPower, short maxSpeed, double maxTurn)
     throw();

  protected:
    const Parameters *  params_;
    Connection&         connection; // encapsulating communication to hardware

  };
};

#endif




