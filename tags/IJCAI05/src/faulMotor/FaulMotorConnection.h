// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef FaulMotorConnection_h
#define FaulMotorConnection_h

#include "faulTty/FaulControllerConnection.h"
#include "sparrow/SparrowConnection2003.h"

#include "miro/Synch.h"

//------------------------ FaulMotorClass ---------------------------//

namespace FaulMotor
{
  // forward declaration
  class Consumer;
  class Parameters;

  //! Class for using the faulhaber motor controllers.
  /**
   * The actual connection is etither held by two serial devices
   * or by the can device. So this connection is not derived from
   * DevConnection, itself.
   */
  class Connection
  {
  public:
    //--------------------------------------------------------------------------
    // public methods
    //--------------------------------------------------------------------------

    //! Connection to the faulhaber device.
    Connection(ACE_Reactor * _reactor,
	       Consumer * _consumer,
	       Sparrow::Connection2003 * _connection2003);
    //! Virtual destructor.
    virtual ~Connection();

    //-----------------//
    // motor functions //
    //-----------------//
    //void stop();                              // stop, motor remain enabled

    //! Enable binary controller commands.
    void enableBinary();
    //! Set the speed for the left and right wheel.
    /** 
     * Acceleration values are calculated accordingly.  They are bound
     * by the parameter maxAccelertion.
     */
    void setSpeed(short speedL, short speedR);
    //! Query a ticks message
    void getTicks();
    //! Initialize the left an right ticks.
    void setPosition(int left, int right);
    //! Enable motors.
    void enable();
    //! Disable motors.
    void disable();
    //! Emergency stop.
    void jmp3();
    //! Send the speed values to the motor controllers.
    /** 
     * In order to adhere to the fragile timing protocol of the
     * Faulhaber controller, commands are not send directly, but are
     * timer triggerd.
     */
    void deferredSetSpeed();
    
  protected:
    //--------------------------------------------------------------------------
    // protected methods
    //--------------------------------------------------------------------------

    // Serial line writing
    void protectedDeferredSetSpeed();

    //--------------------------------------------------------------------------
    // protected data
    //--------------------------------------------------------------------------

    const Parameters * params_;

    //! The device connection for the left wheel.
    FaulController::Connection * leftWheel_;
    //! The device connection for the right wheel.
    FaulController::Connection * rightWheel_;
    //! The consumer for processing odometry replies
    Consumer * consumer;
    //! The can bus based connection.
    Sparrow::Connection2003 * connection2003;

    //! New speed for left wheel.
    int newSpeedL;
    //! New speed for right wheel.
    int newSpeedR;
    //! New acceleration for left wheel.
    int newAccL;
    //! New acceleration for right wheel.
    int newAccR;

    int prevSpeedL;
    int prevSpeedR;
    int prevAccL;
    int prevAccR;

    //! Flag remembering if motors are disabled.
    bool disabled_;

    //! Mutex to protect for multiple parallel writes.
    /* Seems to be superfluent, so. */
    Miro::Mutex mutex_;

  public:
    static unsigned int gotTicks_;
  };
}

#endif

