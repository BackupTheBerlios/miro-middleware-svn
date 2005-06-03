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
#ifndef PioneerConnection_h
#define PioneerConnection_h

#include "psos/PsosConnection.h"
#include "pioneer/CanonMessage.h"

//------------------------ PioneerClass ---------------------------//

namespace Pioneer
{
  // forward declaration
  class Consumer;
  class Parameters;

  /**
   * Class for using the Pioneer robot
   */
  class Connection : public Psos::Connection
  {
    typedef Psos::Connection Super;

  public:
    Connection(ACE_Reactor * _reactor, 
	       Psos::EventHandler * _eventHandler,
	       Consumer * _consumer);
    virtual ~Connection();

    //-----------------//
    // motor functions //
    //-----------------//
    void stop();                              // stop, motor remain enabled
    void setSpeed(short speed);
    void setSpeed2(short left, short right);  // set both wheel's speed
    void setRotVel(short rotvel);             // set rotation velocity
    void setSpeedRot(short speed, short rot); // set 
    void setHeading(short head);              // turn to absolute position
    void turn(short turn_deg);                // turn about turn_deg degrees
    void translate(short trans_mm);           // translate +/- mm forward/back

    //-----------------//
    // servo functions //
    //-----------------//
    void setServo(unsigned short degrees);       // turn servo to abs. angle (degree)
    void setServo(short degrees);       // turn servo to +/- 90� from middle puls
    void setServoMidPulse(unsigned short pulse); // servo middle pulse (def: 880)

    short getServo()    { return servoDegrees; }
    void servosOff();

    //------------------//
    // kicker functions //
    //------------------//
    void kick(unsigned short ms);             // kicks for ms milliseconds

    //------------------
    // motor stalled predicates
    // ---------------------
    //    bool  isStalledRight()     { return info.bumperl>0; }  // left<->right !!!!!
    //    bool  isStalledLeft()      { return info.bumperr>0; }
    //    bool  isStalled()          { return (info.bumperl>0) || (info.bumperr>0); }
    //    bool  isStalledBoth()      { return (info.bumperl>0) && (info.bumperr>0); }

    //    void setPosition(short x, short y, short theta_rad);
    //    void setOdometryRate(unsigned short ms);

    void sendCamera(const Pioneer::CameraMessage& cameraMessage);
    void getCamera(unsigned short length); //get length bytes from AUX1

    void gripperCommand(unsigned short command); //send a command to the gripper
    void gripperValue(short value); //send a value to the gripper

    void setTCMMode(short value); // send a command to the TCM2 module
  protected:
    Consumer * consumer;

    Miro::Mutex       stateMutex;
    short             rotSpeed;
    unsigned short    servoMidPulse;     // pulse width of mid position (ca. 880)
    unsigned short    servoDegrees;

    // short             transSpeed;     // speed set in last setSpeed command
    // short             servoPulse;     // servo position set in last setServo

    const Parameters * params_;
  };

};

#endif
