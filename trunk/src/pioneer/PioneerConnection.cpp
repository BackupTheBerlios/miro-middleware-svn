// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

// This implements an object-oriented user interface to the Pioneer
// robot.

#include "PioneerConnection.h"
#include "PioneerConsumer.h"
#include "CanonPanTiltImpl.h"
#include "Parameters.h"

#include "psos/PsosMessage.h"

#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif

using namespace Psos;

using Miro::Guard;

namespace Pioneer
{
  //------------------------//
  //----- constructors -----//
  //------------------------//

  Connection::Connection(ACE_Reactor* _reactor, 
			 EventHandler* _eventHandler,
			 Consumer * _consumer) : 
    Super(_reactor, _eventHandler, *Parameters::instance()),
    consumer(_consumer),
    servoMidPulse(129),    // default for servo middle position pulse (org 800)
                           // 129 getestet 19.12.2001 TODO: make it a parameter...
    params_(Parameters::instance())
  { 
    DBG(cout << "Constructing PioneerConnection." << endl);
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//

  Connection::~Connection()
  { 
    DBG(cout << "Destructing PioneerConnection." << endl);

    //if there is a camera, close the connection
    if (consumer->pCanonPanTilt!=NULL) consumer->pCanonPanTilt->done();
  }

  //-------------------//
  //----- methods -----//
  //-------------------//

  //----------- commands ----------- //

  // stop,
  // motors remain enabled
  void
  Connection::stop() {
    Message stopMessage(SF_COMSTOP);
    writeMessage(stopMessage);
  }

  // set absolute speed in mm/sec,
  // positive values are forward, negatives backward
  void
  Connection::setSpeed(short speed)				// lauft 3.12.01
  {
    Message speedMessage(SF_COMVEL, speed); // build speed packet
    writeMessage(speedMessage);             // send it
    //cout << "setSpeed speed:_ " << speed << endl
    //        << speedMessage << endl;
  }

  // set absolute speed in mm/sec for both wheels
  // positive values are forward, negatives backward
  void
  Connection::setSpeed2(short left, short right)
  {
    Message speed2Message(SF_COMVEL2,  // build speed packet
			  (char)((double)right / params_->vel2Divisor), 
			  (char)((double)left / params_->vel2Divisor));
    writeMessage(speed2Message);                        // send it
    //cout << "setSpeed2 right_ left_:_ " << right << "--"<< left << endl
    //        << speed2Message << endl;
  }

  void
  Connection::setSpeedRot(short speed, short rotVel)
  {
    setSpeed(speed);
    setRotVel(rotVel);
  }

  // rotate with a given velocity of up to +-255 degrees/sec
  // positive values are to the ..., negative to the ...
  void
  Connection::setRotVel(short rotvel)
  {
    Message speedMessage(SF_COMRVEL, rotvel); // build rotvel packet
    writeMessage(speedMessage);               // send it
    //cout << "setRotVel:_ " << rotvel << endl
    //       << speedMessage << endl;
  }

  void
  Connection::setHeading(short head)
  {
    Message headMessage(SF_COMHEAD, head);          // build heading packet
    writeMessage(headMessage);               // send it
  }

  // turns about relhead in deg
  void
  Connection::turn(short relhead)
  {
    Message relheadMessage(SF_COMDHEAD, relhead); // build rel. heading packet
    writeMessage(relheadMessage);        // send it
  }

  // translate about reltrans in mm
  void
  Connection::translate(short reltrans)
  {
    Message reltransMessage(SF_COMMOVE, reltrans); // build rel. trans packet
    writeMessage(reltransMessage);        // send it
  }

  void
  Connection::setServo(unsigned short degrees)
  {
    Guard guard(stateMutex);

    if (degrees != servoDegrees) {
      Message servoMessage(SF_COMPTUPOS, (unsigned short) (servoMidPulse - (degrees * 1.03888))); // 1.0388 korrektur
      writeMessage(servoMessage);
      servoDegrees = degrees;
      //cout << "ServoPos:_ " << degrees << endl << servoMessage << endl;
    }
 }
  void
  Connection::setServo(short degrees) // uli 19.12.2001
  {
    Guard guard(stateMutex);

    if (degrees != servoDegrees) {
      Message servoMessage(SF_COMPTUPOS, (unsigned short) (servoMidPulse - (degrees * 1.03888))); // 1.0388 korrektur
      writeMessage(servoMessage);                                                                 //konstante in psosDevice
      servoDegrees = degrees;
      //cout << "ServoPos:_ " << degrees << endl << servoMessage << endl;
    }
  }
  void
  Connection::setServoMidPulse(unsigned short pulse)
  {
    Guard guard(stateMutex);
    if (pulse != servoMidPulse) {
      //setServo(pulse);                               // re-position servo
      servoMidPulse = pulse;
      
    }
  }

  void
  Connection::kick(unsigned short ms)
  {
    Message kickMessage(34, ms);             // build kick packet
    writeMessage(kickMessage);               // send it
  }

  void Connection::sendCamera(Canon::Message cameraMessage)
  {
    getCamera(0); //flush buffer
    Message camMsg(SF_COMTTY2,(char*)cameraMessage.buffer(),cameraMessage.length());
    writeMessage(camMsg);
    getCamera(6); //get minimum answer 
  }

  void Connection::getCamera(unsigned short length)
  {
    Message answer(SF_COMGETAUX,length);
    writeMessage(answer);
  }

  void Connection::gripperCommand(unsigned short command)
  {
    Guard guard(stateMutex);
    Message com(SF_COMGRIPPER,command);
    writeMessage(com);
  }

  void Connection::gripperValue(short value)
  {
    Guard guard(stateMutex);
    Message com(SF_COMGVAL,value);
    writeMessage(com);
  }

};
