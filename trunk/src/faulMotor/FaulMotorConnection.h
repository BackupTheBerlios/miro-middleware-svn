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

//------------------------ FaulMotorClass ---------------------------//

namespace FaulMotor
{
  // forward declaration
  class Consumer;
  class Parameters;

  /**
   * Class for using the FaulMotor robot
   */
  class Connection : public FaulTty::Connection
  {
    typedef FaulTty::Connection Super;

  public:
    Connection(ACE_Reactor * _reactor, 
	       FaulTty::EventHandler * _eventHandler,
	       Consumer * _consumer);
    virtual ~Connection();

    //-----------------//
    // motor functions //
    //-----------------//
    //void stop();                              // stop, motor remain enabled
    void setSpeed(short speed);
    void setSpeed(short speedL, short speedR);
    void getSpeed();
    void getTicks();
    void setBefehl(char* befehl);

  protected:
    Consumer * consumer;

    Miro::Mutex       stateMutex;
    short             rotSpeed;

    const Parameters * params_;
  };

};

#endif

