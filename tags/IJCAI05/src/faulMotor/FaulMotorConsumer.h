// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef FaulMotorConsumer_h
#define FaulMotorConsumer_h

#include "miro/DevConsumer.h"
#include "idl/MotionStatusC.h"

#include <ace/Time_Value.h>

// forward declarations
namespace Miro {
  class OdometryImpl;
}

namespace FaulMotor
{
  // forward declarations
  class Parameters;
  class Connection;

  //! Class integrating data from the faulhaber controller
  class Consumer : public Miro::DevConsumer
  {
    //--------------------------------------------------------------------------
    // private types
    //--------------------------------------------------------------------------
    typedef Miro::DevConsumer Super;

  public:
    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------
    Consumer(Miro::OdometryImpl * _pOdometry = NULL);
    virtual ~Consumer();

    //! inherited interface
    virtual void handleMessage(const Miro::DevMessage * _message);

  protected:
    //--------------------------------------------------------------------------
    // protected methods
    //--------------------------------------------------------------------------
    void integrateBinary();
    void integrateAscii();
    void odometryUpdate(double _dL, double _dR, double _deltaT);

    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------
    FaulMotor::Parameters * params_;
    Miro::OdometryImpl * pOdometry_;
    Miro::MotionStatusIDL status_;

    //! Counter to skip the first initializing odometry ticks.
    int init_;

    double xPos_;
    double yPos_;

    int ticksL_;
    int ticksR_;
    int prevTicksL_;
    int prevTicksR_;

    int deltaTicksL_;
    int deltaTicksR_;
    int clockL_;
    int clockR_;
    
    ACE_Time_Value timeStampL_;
    ACE_Time_Value timeStampR_;
    ACE_Time_Value prevTimeStampL_;
    ACE_Time_Value prevTimeStampR_;

    double wheelBase_;
    int oddWheel_;

    //--------------------------------------------------------------------------
    // protected constants
    //--------------------------------------------------------------------------

    // the pace of the faulhaber controller
    static double const CLOCK_2_SEC;
  };
}
#endif

