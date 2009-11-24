// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
#include "pioneer/PioneerConnection.h"
#include "pioneer/Parameters.h"
#include "pioneer/PioneerConsumer.h"
#include "pioneer/PioneerStallImpl.h"
#include "pioneer/CameraMessage.h"
#include "pioneer/CanonPanTiltImpl.h"

#include "pioneer/CanonCameraControlImpl.h"
#include "pioneer/GripperImpl.h"
#include "pioneer/TCM2Impl.h"

#include "psos/PsosEventHandler.h"

#include "miro/ReactorTask.h"
#include "miro/IO.h"
#include "miro/Utils.h"
#include "miro/Configuration.h"
#include "miro/TimeHelper.h"
#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"

#ifdef MIRO_HAS_DEPRECATED
#include "pioneer/CanonCameraImpl.h"
#endif

#include <ace/Signal.h>

#include <iostream>
#include <cstdio>
 
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

bool canceled = false;

using Miro::deg2Rad;

// we need to shut down the pioneer board properly on sig term
class Event : public ACE_Event_Handler
{
public:
  Event() {}
  //  private:
  virtual ~Event() {}
public:
  
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
};


int
Event::handle_signal(int, siginfo_t *, ucontext_t *)
{
  canceled = true;
  return 0;
}

 
struct Service
{
  Miro::ReactorTask reactorTask;
  Miro::RangeSensorImpl * pRangeSensorImpl;
  Miro::OdometryImpl * pOdometryImpl;
  Miro::BatteryImpl * pBatteryImpl;
  Pioneer::CameraAnswer * pCameraAnswer;
  Pioneer::Consumer * pConsumer;
  Pioneer::StallImpl * pStallImpl;
  Pioneer::TCM2Impl * pTCM2Impl;
  Canon::CanonPanTiltImpl * pPanTiltImpl;
#ifdef MIRO_HAS_DEPRECATED
  Canon::CanonCameraImpl canonCamera;
#endif
  Miro::CameraControlImpl * pCameraControl;
  Miro::GripperImpl gripper;
  Psos::EventHandler * pEventHandler;
  Pioneer::Connection connection;
  
  Service();
  ~Service();
};


Service::Service() :
  reactorTask(),
  pRangeSensorImpl(new Miro::RangeSensorImpl(Pioneer::Parameters::instance()->sonarDescription)),
  pOdometryImpl(new Miro::OdometryImpl(NULL)),
  pBatteryImpl(new Miro::BatteryImpl()),
  pCameraAnswer(new Pioneer::CameraAnswer()),
  pConsumer(new Pioneer::Consumer(pRangeSensorImpl, NULL, NULL, NULL, pOdometryImpl, NULL, NULL, NULL, NULL, pCameraAnswer)),
  pStallImpl(new Pioneer::StallImpl()),
  pTCM2Impl(new Pioneer::TCM2Impl(Pioneer::Parameters::instance()->tcm2Params, NULL)),
#ifdef MIRO_HAS_DEPRECATED
  canonCamera(connection, static_cast<Canon::Answer *>(pCameraAnswer)),
#endif
  gripper(connection),
  pEventHandler(new Psos::EventHandler(pConsumer,
				       connection, 
				       Pioneer::Parameters::instance())),
  connection(reactorTask.reactor(), pEventHandler, pConsumer)
{
  Canon::Answer * pAnswer = static_cast<Canon::Answer *>(pCameraAnswer);
  pPanTiltImpl=new
    Canon::CanonPanTiltImpl(connection, pAnswer,
			    Pioneer::Parameters::instance()->panTiltParams,
			    Pioneer::Parameters::instance()->cameraParams.upsideDown);
}

Service::~Service()
{
  if (pPanTiltImpl)
    delete(pPanTiltImpl);
}

void gripperMenu(Service& service)
{
  bool loop=true;
  char c;
  std::string str;

  while(loop) {
    cout << endl
	 << "*****  Gripper menu:  *****"<< endl
	 << "0 - stop Grip" << endl
	 << "1 - set Grip pressure" << endl
	 << "2 - open Grip" << endl
	 << "3 - close Grip" << endl
	 << "4 - raise Grip" << endl
	 << "5 - lower Grip" << endl
	 << "6 - store Grip " << endl
	 << "7 - deploy Grip " << endl
	 << "8 - move Grip (time)" << endl
	 << endl << "x - back" << endl;
    cin >> str;
    c = str[0];
    printf("\033[2J");             // clear screen
    printf("\033[0;0f");           // set cursor
    
    switch (c)
      {
      case '0':
	{
	  service.gripper.stopGrip();
	  break;
	}
      case '1' :
	{
	  int value;
	  cout << "percentage: " << endl;
	  cin >> value;

	  service.gripper.setGripPressure(value);
	  break;
	}
      case '2' :
	{
	  service.gripper.openGrip();
	  break;
	}
      case '3' :
	{
	  service.gripper.closeGrip();
	  break;
	}
      case '4' :
	{
	  service.gripper.raiseGrip();
	  break;
	}
      case '5' :
	{
	  service.gripper.lowerGrip();
	  break;
	}
      case '6':
	{
	  service.gripper.storeGrip();
	  break;
	}
      case '7':
	{
	  service.gripper.deployGrip();
	  break;
	}
      case '8' :
	{
	  int value;
	  cout << "time (ms): " << endl;
	  cin >> value;

	  service.gripper.moveGrip(value);
	  break;
	}
      case 'X':
      case 'x' : loop=false;break;
      default: cout << "gibts doch gar net!!!" << endl;
	
      }//switch(c)
    
  } //while(loop)

}


void cameraMenu(Service& service)
{
  bool loop=true;
  char c;
  std::string str;

  cout << "camera limits:" << endl;

  Miro::PanTiltLimitsIDL limits;
  limits=service.pPanTiltImpl->getPanTiltLimits();

  cout << "Pan min/max value: " << limits.pan.minAngle << " / " << limits.pan.maxAngle << endl;
  cout << "Tilt min/max value: " << limits.tilt.minAngle << " / " << limits.tilt.maxAngle << endl;

  while(loop) {
    cout << endl
	 << "*****  Camera menu:  *****"<< endl
	 << "1 - set Pan/Tilt position" << endl
	 << "2 - set Pan/Tilt position (with wait)" << endl
	 << "3 - set Pan position" << endl
	 << "4 - set Tilt position" << endl
	 << "5 - get Pan/Tilt position" << endl
	 << "6 - set Pan/Tilt speeds " << endl
	 << "7 - set Zoom position" << endl
	 << "8 - get Zoom position" << endl
	 << "9 - set Focus position" << endl
	 << "0 - get Focus position" << endl
	 << "a - set AutoFocus" << endl
	 << "b - set AE Lock OFF" << endl
	 << "c - set AE Lock ON" << endl
#ifdef MIRO_HAS_DEPRECATED
	 << "d - set AE Value" << endl
	 << "e - set Iris Value" << endl
#endif
         << "f - set Shutter Speed" << endl
#ifdef MIRO_HAS_DEPRECATED
         << "g - set AGC Gain " << endl
         << "h - set White Balance Auto" << endl
         << "i - set White Balance Lock" << endl 
	 << "k - set White Balance Manual" << endl
	 << "l - set White Balance Value" << endl 
#endif
	 << endl << "x - back" << endl;
    cin >> str;
    c = str[0];
    printf("\033[2J");             // clear screen
    printf("\033[0;0f");           // set cursor
    
    switch (c)
      {
      case '1' :
	{
	  double angle;
	  Miro::PanTiltPositionIDL pos;
	  cout << "pan (deg): " << endl;
	  cin >> angle;
	  pos.panValue=deg2Rad(angle);
	  cout << "tilt (deg): " << endl;
	  cin >> angle;
	  pos.tiltValue=deg2Rad(angle);
	  
	  service.pPanTiltImpl->setPosition(pos);
	  break;
	}
      case '2' :
	{
	  double angle;
	  Miro::PanTiltPositionIDL pos;
	  cout << "pan (deg): " << endl;
	  cin >> angle;
	  pos.panValue=deg2Rad(angle);
	  cout << "tilt (deg): " << endl;
	  cin >> angle;
	  pos.tiltValue=deg2Rad(angle);
	  
	  service.pPanTiltImpl->setWaitPosition(pos);
	  break;
	}
      case '3' :
	{
	  double angle;
	  cout << "pan (deg): " << endl;
	  cin >> angle;
	  
	  service.pPanTiltImpl->setPan(deg2Rad(angle));
	  break;
	}
      case '4' :
	{
	  double angle;
	  cout << "tilt (deg): " << endl;
	  cin >> angle;
	  
	  service.pPanTiltImpl->setTilt(deg2Rad(angle));
	  break;
	}
      case '5' :
	{
	  Miro::PanTiltPositionIDL pos=service.pPanTiltImpl->getPosition();
	  cout << "Pan: " << Miro::rad2Deg(pos.panValue) << "�" << endl;
	  cout << "Tilt: " << Miro::rad2Deg(pos.tiltValue) << "�" << endl;
	  break;
	}
      case '6':
	{
	  Miro::CanonPanTiltSpdAccIDL spd;
	  double angle;
	  spd=service.pPanTiltImpl->getSpdAcc();
	  
	  cout 
	    << "Current pan speed:" << spd.targetpanspeed << endl
	    << "Current tilt speed:" << spd.targettiltspeed << endl
	    << "Minimum pan speed:" << spd.panminspeed << endl
	    << "Maximum pan speed:" << spd.panmaxspeed << endl
	    << "Minimum tilt speed:" << spd.tiltminspeed << endl
	    << "Maximum tilt speed:" << spd.tiltmaxspeed << endl
	    << endl;
	  cout << "New pan speed (deg/s): " << endl;
	  cin >> angle;
	  spd.targetpanspeed=deg2Rad(angle);
	  cout << "New tilt speed (deg/s): " << endl;
	  cin >> angle;
	  spd.targettiltspeed=deg2Rad(angle);
	  service.pPanTiltImpl->setSpdAcc(spd);
	  break;
	}
      case '7':
	{
	  int zoom;
	  cout << "New zoom position (0-100%):" << endl;
	  cin >> zoom;
#ifdef MIRO_HAS_DEPRECATED	  
	  service.canonCamera.setZoom(zoom);
#endif
	  break;
	}
      case '8':
	{
#ifdef MIRO_HAS_DEPRECATED
	  cout << "current zoom factor: " << service.canonCamera.getZoom();
#endif
	  break;
	}
      case '9':
	{
	  int focus;
	  cout << "New focus position (0-100%):" << endl;
	  cin >> focus;
#ifdef MIRO_HAS_DEPRECATED
	  service.canonCamera.setFocus(focus);
#endif
	  break;
	}
      case '0':
	{
#ifdef MIRO_HAS_DEPRECATED
	  cout << "current focus factor: " << service.canonCamera.getFocus();
#endif
	  break;
	}
      case 'a':
	{
#ifdef MIRO_HAS_DEPRECATED
	  service.canonCamera.autoFocus();
#endif
	  break;
	}
      case 'b':
	{
#ifdef MIRO_HAS_DEPRECATED
	  service.canonCamera.setAEoff();
#endif
	  break;
	}
      case 'c':
	{
#ifdef MIRO_HAS_DEPRECATED 
	  service.canonCamera.setAEon();
#endif
	  break;
	}
      case 'd':
	{
	  int aeValue;
	  cout << "New AE-value(16-255):" << endl;
	  cin >> aeValue;

#ifdef MIRO_HAS_DEPRECATED
	  service.canonCamera.setAE(aeValue);
#endif
	  break;
	}
      case 'e':
	{
	  int irisValue;
	  cout << "New Iris-value(2-10):" << endl;
	  cin >> irisValue;

#ifdef MIRO_HAS_DEPRECATED
	  service.canonCamera.setIrisAssignment(irisValue);
#endif
	  break;
	}
      case 'f':
        {
          int shutterValue;
          cout << "New Shutter Speed(1-25):" << endl;
          cin >> shutterValue;

#ifdef MIRO_HAS_DEPRECATED
          service.canonCamera.setShutterSpeed(shutterValue);
#endif
          break;
        }
      case 'g':
        {
          int gainValue;
          cout << "New AGC Gain-value(0-255):" << endl;
          cin >> gainValue;

#ifdef MIRO_HAS_DEPRECATED
          service.canonCamera.setAGCGain(gainValue);
#endif
          break;
        }
      case 'h':
        {
#ifdef MIRO_HAS_DEPRECATED
          service.canonCamera.setWBauto();
#endif
          break;
        }
      case 'i':
        {
#ifdef MIRO_HAS_DEPRECATED
          service.canonCamera.setWBlock();
#endif
          break;
        }
      case 'k':
        {
#ifdef MIRO_HAS_DEPRECATED
          service.canonCamera.setWBmanual();
#endif
          break;
        }
      case 'l':
        {
          int wbValue;
          cout << "New White Balance-value(0-255):" << endl;
          cin >> wbValue;

#ifdef MIRO_HAS_DEPRECATED
          service.canonCamera.setWBvalue(wbValue);
#endif
          break;
        }

      case 'X':
      case 'x' : loop=false;break;
      default: cout << "gibts doch gar net!!!" << endl;

      }//switch(c)

  } //while(loop)
}

int main(int argc, char* argv[])
{
  Miro::Configuration::init(argc,argv);

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParameters = Miro::RobotParameters::instance();
  Pioneer::Parameters * pParams = Pioneer::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
  config->setSection("Robot");
  config->getParameters("Miro::RobotParameters", *robotParameters);
  config->setSection("ActiveMedia");
  config->getParameters("PioneerBoard", *pParams);

  delete config;

#ifdef DEBUG
  cout << "  Pioneer parameters:" << endl << *pParams << endl;
#endif





  // Initialize server daemon.
  Service service;
  Event event;

  // Signal set to be handled by the event handler.
  ACE_Sig_Set sigs;

  ACE_Time_Value ace_time;
  
  bool loop = true;  
  char c;
  std::string str;
  short k;
  int i;
    

  // register Signal handler for Ctr+C
  sigs.sig_add(SIGINT);
  sigs.sig_add(SIGTERM);

  if (service.reactorTask.reactor()->register_handler(sigs, &event) == -1) {
    throw Miro::ACE_Exception(errno, "failed to register signal handler");
  }

  service.reactorTask.open(NULL);

  do {
    cout << "waiting for synch" << endl;
  }
  while (!canceled && !service.connection.waitSynch(ACE_Time_Value(2)));

  try 
    {
      Miro::RangeGroupEventIDL_var  pSonarEvent;                    
      Miro::PositionIDL odoData;
      
      pSonarEvent = service.pRangeSensorImpl->getGroup(0);
      service.pOdometryImpl->setPosition(odoData);
      odoData = service.pOdometryImpl->getWaitPosition();
      k=100;
      odoData = service.pOdometryImpl->getWaitPosition();
      cout << "Odo:__" << odoData << endl;	
      
      //service.connection.setSpeed(0);
      while(loop)
	{
	  cout << endl
	       << "*****  Menu:  *****"<< endl
	       << "1 - motor on ! " << endl
	       << "2 - motor on (with time)" << endl
	       << "3 - turn" << endl
	       << "4 - turn and drive" << endl
	       << "6 - set servo" << endl
	       << "7 - status (Sonar and Stall infos)" << endl
	       << "8 - Odo status" << endl
	       << "9 - set rotation velocity" << endl
	       << "0 - all motors off!!!";
	  if (Pioneer::Parameters::instance()->camera) cout << endl << "a - camera Options";
	  cout << endl
	       << "b - gripper Options" << endl
	       << endl << "x - Program end!!!" << endl;
	  cin >> str;
          c = str[0];
	  printf("\033[2J");             // clear screen
	  printf("\033[0;0f");           // set cursor

	 switch (c)
	 {
	 case '1' : 
	   { 
	     cout << "what speed? (mm/sec) " << endl;
	     cin >> k;
	     service.connection.setSpeed(k);break;
	   }
	   
	 case '2' : 
	   { 
	     cout << "how long?  (mmsec)" << endl;
	     cin >> i;
	     ace_time.msec(i);
	     cout << "what speed? (mm/sec)  " << endl;
	     cin >> k;
	     service.connection.setSpeed(k);
	     ACE_OS::sleep(ace_time);
	     service.connection.setSpeed(0);break;
	   }
	   
	 case '3' : 
	   { 
	     cout << "turn heading in degrees: " << endl;
	     cin >> k;
	     service.connection.setSpeed(0); // stop motors
	     service.connection.turn(k);
	     break;
	   }
	   
	 case '4' : 
	   { 
	     cout << "what speed? (mm/sec):  " << endl;
	     cin >> k;
	     cout << "degrees ?  " << endl;
	     cin >> i;
	     service.connection.setSpeedRot(k, i);break;
	   }
	   
	 case '5' :                                 // muss ich noch testen
	   {
	     cout << "Enter Servo middle pules! (initial 129) " << endl;
	     cin >> k;
	     service.connection.setServoMidPulse((unsigned short)k);
	     cout << "Servo auf null gestellt mit: " << k << endl;
	     break;
	   }
	   
	 case '6' :  
	   { 
	     cout << "Servosetting:" << endl 
		  << "  0: middle Setting " << endl
		  << "  +/- 90� " << endl;
	     cin >> k;  
	     service.connection.setServo(k);
	     break;
	   } 
	   
	 case '7' :  
	   {  
	     pSonarEvent = service.pRangeSensorImpl->getGroup(0);
	     for (i=0;i<=7;i++)
	       {  cout << "Sonar"  << i << ":__" << pSonarEvent->range[i] << endl; }
	     k= service.pStallImpl->getStalledWheels();
	     if (k==1) cout <<"only left wheel stalled! " << endl;
	     if (k==2) cout <<"only right wheel stalled! " << endl;
	     if (k==3) cout <<"left and right wheel stalled!!! " << endl;
	     cout << "Battery (volt):  " << service.pBatteryImpl->getVoltage() << endl; 
	     break;
	   }
	   
	 case '8' :
	   { 
	     odoData = service.pOdometryImpl->getPosition();
	     cout << "Odo:__" << odoData << endl;break;
	   }
	   
	 case '9' : 
	   {  
	     cout << "set rotation Velocity: " << endl;
	     cin >> k;
	     service.connection.setRotVel(k);break;
	   }
	   
	 case '0' : 
	   {
	     service.connection.setSpeed(0);
	     break;
	   }
	 case 'a' :
	   {
	     if (Pioneer::Parameters::instance()->camera) cameraMenu(service);
	     break;
	   }
	 case 'b' :
	   {
	     gripperMenu(service);
	     break;
	   }
	 case 'X':
	 case 'x' : loop=false;break;
	 default: cout << "gibts doch gar net!!!" << endl;
	   
	 }//switch(c)

       } //while(loop)
  }
  catch(const CORBA::Exception& e) {
    cerr << "CORBA exception thrown:" << e << endl;
  }
  catch (const Miro::Exception & e) {
    cerr << "Miro exception thrown:" << e << endl;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
  }

  service.connection.close();

  service.reactorTask.cancel();
  return 0;
}





