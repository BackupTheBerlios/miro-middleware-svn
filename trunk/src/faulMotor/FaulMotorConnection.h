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

  /**
   * Class for using the FaulMotor robot
   */
  class Connection
  {
  public:
    Connection(ACE_Reactor * _reactor,
	       Consumer * _consumer,
	       Sparrow::Connection2003 * _connection2003);
    virtual ~Connection();

    //-----------------//
    // motor functions //
    //-----------------//
    //void stop();                              // stop, motor remain enabled

    //! Enable binary controller commands.
    void enableBinary();

    void setSpeed(short speedL, short speedR);
    void getSpeed();
    void getTicks();
    void setBefehl(char const * const befehl);
    void setPosition(int left, int right);
    void enable();
    void disable();
    void jmp1();
    void jmp2();
    void jmp3();

    void deferredSetSpeed(ACE_Time_Value const& _now);
    
  protected:
    //! mutex protected write
    void protectedDeferredSetSpeed();

    const Parameters * params_;

    FaulController::Connection * leftWheel_;
    FaulController::Connection * rightWheel_;

    Consumer * consumer;
    Sparrow::Connection2003 * connection2003;

    ACE_Time_Value nextSceduledQuery_;
    bool writeThisRound_;

    int newSpeedL;
    int newSpeedR;
    int newAccL;
    int newAccR;

    int prevSpeedL;
    int prevSpeedR;
    int prevAccL;
    int prevAccR;

    bool disabled_;

    Miro::Mutex mutex_;

  public:
    static unsigned int gotTicks_;
  };
}

#endif

