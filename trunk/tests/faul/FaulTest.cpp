// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 2000, 2001, 2002. 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "faulMotor/FaulMotorConnection.h"
#include "faulMotor/Parameters.h" 
#include "faulMotor/FaulMotorConsumer.h"
//#include "faulMotor/FaulMotorStallImpl.h"
//#include "faulMotor/CanonPanTiltImpl.h"
//#include "faulMotor/CanonCameraImpl.h"
//#include "faulMotor/GripperImpl.h"

#include "faulTty/FaulTtyEventHandler.h"
#include "faulTty/TimerEventHandler.h"

#include "miro/ReactorTask.h"
#include "miro/IO.h"
#include "miro/Utils.h"
#include "miro/TimeHelper.h"
#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"

#include <iostream>
#include <stdio.h>
 
bool canceled = false;

using Miro::deg2Rad;

// we need to shut down the faulMotor board properly on sig term
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
  //Miro::RangeSensorImpl * pRangeSensorImpl;
  Miro::OdometryImpl * pOdometryImpl;
  //Miro::BatteryImpl * pBatteryImpl;
  FaulMotor::Consumer * pConsumer;
  //FaulMotor::StallImpl * pStallImpl;
  //Canon::CanonPanTiltImpl canonPanTiltImpl;
  //Canon::CanonCameraImpl canonCamera;
  //Miro::GripperImpl gripper;
  FaulTty::EventHandler * pEventHandler;
  FaulTty::TimerEventHandler * pTimerEventHandler;
  FaulMotor::Connection connection;

  Service();
};


Service::Service() :
  reactorTask(),
  //pRangeSensorImpl(new Miro::RangeSensorImpl(FaulMotor::Parameters::instance()->sonarDescription)),
  pOdometryImpl(new Miro::OdometryImpl(NULL)),
  //pBatteryImpl(new Miro::BatteryImpl()),
  pConsumer(new FaulMotor::Consumer(NULL, pOdometryImpl)), //&canonPanTiltImpl)),
  //pStallImpl(new FaulMotor::StallImpl()),
  //canonPanTiltImpl(connection, FaulMotor::Parameters::instance()->cameraUpsideDown),
  //canonCamera(connection, canonPanTiltImpl.getAnswer()),
  //gripper(connection),
  pEventHandler(new FaulTty::EventHandler(pConsumer, connection)),
  pTimerEventHandler(new FaulTty::TimerEventHandler(connection)),
  //pTimerEventHandler(new Psos::TimerEventHandler(connection)),
  connection(reactorTask.reactor(), pEventHandler, pConsumer)
{}



int main(int argc, char* argv[])
{
  // Parameters to be passed to the services
  FaulMotor::Parameters * pParams = FaulMotor::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
  config->setSection("ActiveMedia");
  config->getParameters("FaulMotorBoard", *pParams);
  delete config;

#ifdef DEBUG
  cout << "  faulMotor parameters:" << endl << *pParams << endl;
#endif


  // Initialize server daemon.
  Service service;
  Event event;

  // Signal set to be handled by the event handler.
  ACE_Sig_Set sigs;

  ACE_Time_Value ace_time;

  bool loop = true;
  char c;
  char* eing;
  std::string str;
  short k, l;
  long uptimer;
  int i;


  // register Signal handler for Ctr+C
  sigs.sig_add(SIGINT);
  sigs.sig_add(SIGTERM);

  if (service.reactorTask.reactor()->register_handler(sigs, &event) == -1) {
    throw Miro::ACE_Exception(errno, "failed to register signal handler");
  }
  cout << "updaterate? (msec) " << endl;
	     cin >> uptimer;
  ACE_Time_Value tv(0,uptimer);

  service.reactorTask.reactor()->schedule_timer(service.pTimerEventHandler, NULL, tv ,tv);


  service.reactorTask.open(NULL);

 /*do {
    cout << "waiting for synch" << endl;
  }
  while (!canceled && !service.connection.waitSynch(ACE_Time_Value(2)));
*/
  try
    {
      Miro::RangeGroupEventIDL_var  pSonarEvent;
      Miro::PositionIDL odoData;
      cout << "hier1" << endl;
      /*pSonarEvent = service.pRangeSensorImpl->getGroup(0);
      service.pOdometryImpl->setPosition(odoData);
      odoData = service.pOdometryImpl->getWaitPosition();
      k=100;
      //odoData = service.pOdometryImpl->getWaitPosition();
      cout << "Odo:__" << odoData << endl;
      */
      //service.connection.setSpeed(0);
      while(loop)
	{
	  cout << endl
	       << "*****  Menu:  *****"<< endl
	       << "1 - motor on (L/R)! " << endl
	       << "2 - motor on " << endl
	       << "3 - Befehl!!!" << endl
	       /*<< "4 - turn and drive" << endl
	       << "6 - set servo" << endl
	       << "7 - status (Sonar and Stall infos)" << endl
	       << "8 - Odo status" << endl
	       << "9 - set rotation velocity" << endl*/
	       << "0 - all motors off!!!" << endl
	       << "g - get vel" << endl
	       << "p - get pos" << endl
	       << "x - Program end!!!" << endl;
	  cin >> str;
          c = str[0];
	  //printf("\033[2J");             // clear screen
	  //printf("\033[0;0f");           // set cursor

	 switch (c)
	 {
	 case '1' :
	   {
	     cout << "what speed links? (mm/sec) " << endl;
	     cin >> k;
	     cout << "what speed rechts? (mm/sec) " << endl;
	     cin >> l;
	     service.connection.setSpeed(k, l);
	     break;
	   }

	 case '2' :
	   {
	     cout << "what speed? (mm/sec) " << endl;
	     cin >> k;
	     service.connection.setSpeed(k);break;
	     /*
	     cout << "how long?  (mmsec)" << endl;
	     cin >> i;
	     ace_time.msec(i);
	     cout << "what speed? (mm/sec)  " << endl;
	     cin >> k;
	     service.connection.setSpeed(k);
	     ACE_OS::sleep(ace_time);
	     service.connection.setSpeed(0);break;*/
	   }

	 case '3' :
	   {
	     cout << "Befehleingeben: " << endl;
	     cin >> eing;
	     service.connection.setBefehl(eing);
	     //service.connection.turn(k);
	     break;
	   }

	 case '4' :
	   {
	     cout << "what speed? (mm/sec):  " << endl;
	     cin >> k;
	     cout << "degrees ?  " << endl;
	     cin >> i;
	     //service.connection.setSpeedRot(k, i);break;
	   }

	 case '5' :                                 // muss ich noch testen
	   {
	     cout << "Enter Servo middle pules! (initial 129) " << endl;
	     cin >> k;
	     //service.connection.setServoMidPulse((unsigned short)k);
	     cout << "Servo auf null gestellt mit: " << k << endl;
	     break;
	   }

	 case '6' :
	   {
	     cout << "Servosetting:" << endl
		  << "  0: middle Setting " << endl
		  << "  +/- 90° " << endl;
	     cin >> k;
	     //service.connection.setServo(k);
	     break;
	   }

	 case '7' :
	   {
	     /*pSonarEvent = service.pRangeSensorImpl->getGroup(0);
	     for (i=0;i<=7;i++)
	       {  cout << "Sonar"  << i << ":__" << pSonarEvent->range[i] << endl; }
	     k= service.pStallImpl->getStalledWheels();
	     if (k==1) cout <<"only left wheel stalled! " << endl;
	     if (k==2) cout <<"only right wheel stalled! " << endl;
	     if (k==3) cout <<"left and right wheel stalled!!! " << endl;
	     cout << "Battery (volt):  " << service.pBatteryImpl->getVoltage() << endl;*/
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
	     //service.connection.setRotVel(k);break;
	   }

	 case '0' :
	   {
	     service.connection.setSpeed(0);
	     //service.connection.setSpeed(0);
	     //service.connection.stop();
	     break;
	   }
	 case 'a' :
	   {

	     break;
	   }
	 case 'b' :
	   {

	     break;
	   }
	  case 'g' :
	   {
	     service.connection.getSpeed();
	     break;
	   }
	 case 'p' :
	   {
	     service.connection.getTicks();
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





