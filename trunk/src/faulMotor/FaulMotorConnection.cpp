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

// This implements an object-oriented user interface to the FaulMotor
// robot.

#include "FaulMotorConnection.h"
#include "FaulMotorConsumer.h"
#include "Parameters.h"

#include "faulTty/FaulTtyMessage.h"

#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif


namespace FaulMotor
{
  using namespace FaulTty;
  using Miro::Guard;

  //------------------------//
  //----- constructors -----//
  //------------------------//

  Connection::Connection(ACE_Reactor* _reactor, 
			 EventHandler* _eventHandler,
			 Consumer * _consumer) : 
    Super(_reactor, _eventHandler, *Parameters::instance()),
    consumer(_consumer),
    params_(Parameters::instance())
  { 
    DBG(cout << "Constructing FaulMotorConnection." << endl);
    char buffer[20];
    strcpy(buffer,"ac20\r\n\0");
    Message speedMessage(buffer); // build speed packet
    writeMessage(speedMessage);
  }


  //----------------------//
  //----- destructor -----//
  //----------------------//

  Connection::~Connection()
  { 
    DBG(cout << "Destructing FaulMotorConnection." << endl);
  }

  //-------------------//
  //----- methods -----//
  //-------------------//

  //----------- commands ----------- //

  void
  Connection::setSpeed(short speed)
  {
    char strbuff[20];
    char buffer[20];

    if (speed<10) {       // zum bremsen grosse beschl.
	strcpy(buffer,"ac20\r\n\0");
	Message speedMessage(buffer); // build speed packet
    	writeMessage(speedMessage);
    }

    speed = speed *4.2;//* 112;
    strcpy(buffer,"0v\0");
    sprintf(strbuff, "%d", speed);
    strcat(buffer, strbuff);
    strcat(buffer, "\r\n");

    Message speedMessageL(buffer); // build speed packet
    writeMessage(speedMessageL);

    strcpy(buffer,"1v\0");
    speed = -speed;
    sprintf(strbuff, "%d", speed);
    strcat(buffer, strbuff);
    strcat(buffer, "\r\n");

    Message speedMessageR(buffer); // build speed packet
    writeMessage(speedMessageR);             // send it
  }

  void
  Connection::setSpeed(short speedL, short speedR)
  {
    char strbuff[20];
    char buffer[20];

    speedR = speedR *4.2; //* 112;
    speedL = speedL *4.2; // * 112;
    strcpy(buffer,"1v\0");
    sprintf(strbuff, "%d", speedL);
    strcat(buffer, strbuff);
    strcat(buffer, "\r\n");
    Message speedMessageL(buffer);
    writeMessage(speedMessageL);

    strcpy(buffer,"0v\0");
    speedR = -speedR;
    sprintf(strbuff, "%d", speedR);
    strcat(buffer, strbuff);
    strcat(buffer, "\r\n");
    Message speedMessageR(buffer);
    writeMessage(speedMessageR);
  }

  void
  Connection::getSpeed()
  {

    char buffer[10];

    strcpy(buffer,"0gn\r\n\0");
    Message speedMessageL(buffer); // build speed packet
    writeMessage(speedMessageL);             // send it

    strcpy(buffer,"1gn\r\n\0");
    Message speedMessageR(buffer); // build speed packet
    writeMessage(speedMessageR);
  }

  void
  Connection::getTicks()
  {
    char buffer[10];

    strcpy(buffer,"0pos\r\n\0");
    Message speedMessageL(buffer); // build speed packet
    writeMessage(speedMessageL);             // send it

    strcpy(buffer,"1pos\r\n\0");
    Message speedMessageR(buffer); // build speed packet
    writeMessage(speedMessageR);
  }

  void
  Connection::setBefehl(char* befehl)
  {

    strcat(befehl,"\r\n\0");
    Message speedMessageR(befehl);
    writeMessage(speedMessageR);
  }

  void
  Connection::setPos0(short pos)
  {
    char buffer[10];
    char strbuff[20];
    if (pos ==0) {
    	strcat(buffer,"ho");
	}else{
		strcpy(buffer,"0ho\0");
    		sprintf(strbuff, "%d", pos);
    		strcat(buffer, strbuff);
    		strcat(buffer, "\r\n");
	}
    Message speedMessageR(buffer);
    writeMessage(speedMessageR);
  }

  void
  Connection::setPos1(short pos)
  {
    char buffer[10];
    char strbuff[20];
    if (pos ==0) {
    	strcat(buffer,"ho");
	}else{
		strcpy(buffer,"1ho\0");
    		sprintf(strbuff, "%d", pos);
    		strcat(buffer, strbuff);
    		strcat(buffer, "\r\n");
	}
    Message speedMessageR(buffer);
    writeMessage(speedMessageR);
  }


};
