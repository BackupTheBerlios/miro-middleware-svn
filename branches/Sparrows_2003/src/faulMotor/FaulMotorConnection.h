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

#include "faulTty/FaulTtyConnection.h"
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
    void setSpeed(short speed);
    void setSpeed(short speedL, short speedR);
    void getSpeed();
    void getTicks();
    void setBefehl(char const * const befehl);
    void setPosition(int left, int right);
    void enable();
    void disable();

  protected:
    const Parameters * params_;

    FaulTty::Connection * leftWheel_;
    FaulTty::Connection * rightWheel_;

    Consumer * consumer;
    Sparrow::Connection2003 * connection2003;

    double prevSpeedL;
    double prevSpeedR;
    bool disabled_;

    Miro::Mutex mutex_;
  };

};

#endif

