// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "faulMotor/FaulMotorConnection.h"
#include "faulMotor/Parameters.h"
#include "faulMotor/FaulMotorConsumer.h"
#include "faulMotor/TimerEventHandler.h"
#include "sparrow/Parameters.h"
#include "sparrow/SparrowConnection.h"
#include "sparrow/SparrowConnection2003.h"
#include "can/CanEventHandler.h"
#include "sparrow/SparrowConsumer.h"
#include "sparrow/SparrowConsumer2003.h"
//#include "sparrowBase/SparrowServer.h"

#include "faulTty/FaulTtyEventHandler.h"

#include "miro/ReactorTask.h"
#include "miro/IO.h"
#include "miro/Utils.h"
#include "miro/TimeHelper.h"
#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"
#include "miro/Exception.h"
#include "miro/Log.h"

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


struct FaulhaberHardware
{
  // Faulhaber hardware abstraction
  ACE_Reactor * reactor;
  int timerId;
  FaulMotor::Consumer * pConsumer;
  FaulMotor::TimerEventHandler * pTimerEventHandler;
  FaulMotor::Connection connection;

  FaulhaberHardware(ACE_Reactor * _reactor,
		    Miro::OdometryImpl * _pOdometryImpl,
		    Sparrow::Connection2003 * connection2003_);
  ~FaulhaberHardware();
};

FaulhaberHardware::FaulhaberHardware(ACE_Reactor * _reactor,
				     Miro::OdometryImpl * _pOdometryImpl,
				     Sparrow::Connection2003 * connection2003_) :
  reactor(_reactor),
  timerId(-1),
  pConsumer(new FaulMotor::Consumer(_pOdometryImpl)),
  pTimerEventHandler(new FaulMotor::TimerEventHandler(connection)),
  connection(_reactor, pConsumer, connection2003_)
{
  FaulMotor::Parameters * params = 
    FaulMotor::Parameters::instance();
  if (params->odometryPolling) {
    timerId = reactor->schedule_timer(pTimerEventHandler, NULL, 
				      ACE_Time_Value(2),
				      params->odometryPace);
  }
}

FaulhaberHardware::~FaulhaberHardware()
{
  if (timerId != -1)
    reactor->cancel_timer(timerId);
}

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
  Miro::ReactorTask  reactorTask;
  Miro::OdometryImpl * pOdometryImpl;
  FaulhaberHardware * pFaulhaber;

  // Sparrow board hardware abstraction
  Sparrow::Consumer * pConsumer;
  Sparrow::Consumer2003 * pConsumer2003;
  Can::EventHandler * pCanEventHandler;
  Sparrow::Connection * pConnection;
  Sparrow::Connection2003 * pConnection2003;

  Service();
};


Service::Service() :
  reactorTask(),
  pOdometryImpl(new Miro::OdometryImpl(NULL))
{
  if (Sparrow::Parameters::instance()->sparrow2003) {
    pConsumer2003 = new Sparrow::Consumer2003();
    pCanEventHandler = new Can::EventHandler(pConsumer2003, 
					    Sparrow::Parameters::instance());
    pConnection2003 = new Sparrow::Connection2003(reactorTask.reactor(), 
						pCanEventHandler, 
						pConsumer2003);
    pFaulhaber =  new FaulhaberHardware(reactorTask.reactor(), 
					pOdometryImpl,
					pConnection2003);


    pConsumer2003->registerInterfaces(pConnection2003, NULL, NULL,
				      pFaulhaber->pConsumer, NULL);
  }
  else {
    pFaulhaber = new FaulhaberHardware(reactorTask.reactor(), 
				       pOdometryImpl, 
				       NULL);
  }
}


Service * service;

void floodingTest(int _iter, int _timeout)
{
  int sec = _timeout / 1000;
  int usec = _timeout % 1000;
  usec *= 1000;

  ACE_Time_Value t(sec, usec);


  ACE_Time_Value start = ACE_OS::gettimeofday();
  for (int i = 0; i < _iter; ++i) {
    // set random velocity

    double dl = (double)rand() / ((RAND_MAX + 1.0) * .5) - 1.;
    double dr = (double)rand() / ((RAND_MAX + 1.0) * .5) - 1.;


    short l = (short) (dl * 900.);
    short r = (short) (dl * 900.);
    service->pFaulhaber->connection.setSpeed(l,r);

    if (_timeout != 0) {
      ACE_OS::sleep(t);
    }
  }
  ACE_Time_Value stop = ACE_OS::gettimeofday();

  service->pFaulhaber->connection.setSpeed(0, 0);

  std::cout << "elapsed time: " << (stop - start) << "sec" << std::endl;
}


int main(int argc, char* argv[])
{
  Miro::Log::init(argc, argv);

  // Parameters to be passed to the services
  FaulMotor::Parameters * pParams = FaulMotor::Parameters::instance();
  Sparrow::Parameters * pSparrows = Sparrow::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
  config->setSection("Faulhaber");
  config->getParameters("FaulMotor", *pParams);
  config->setSection("Sparrow99");
  config->getParameters("SparrowBoard", *pSparrows);
  delete config;

#ifdef DEBUG
  cout << "  faulMotor parameters:" << endl << *pParams << endl;
#endif


  // Initialize server daemon.
  service = new Service;
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
  int stallId;


  // register Signal handler for Ctr+C
  sigs.sig_add(SIGINT);
  sigs.sig_add(SIGTERM);

  if (service->reactorTask.reactor()->register_handler(sigs, &event) == -1) {
    throw Miro::ACE_Exception(errno, "failed to register signal handler");
  }
  //cout << "updaterate? (usec) " << endl;
  //cin >> uptimer;
  //ACE_Time_Value tv(0,uptimer);
  //ACE_Time_Value t0(0,0);



  // adding a handler for odometry polling

  ACE_Time_Value tv(0, 500000);
  service->reactorTask.reactor()->schedule_timer(service->pFaulhaber->pTimerEventHandler, NULL, tv ,tv);


  // adding a handler for stall detection
  /*
    ACE_Time_Value t0(0,0);
    stallId = service->reactorTask.reactor()->schedule_Timer(service->pTimerEventHandler, NULL, t0, t0);
  */
  // service->connection.setStallId(stallId);

  service->reactorTask.open(NULL);

  try
  {
    Miro::RangeGroupEventIDL_var  pSonarEvent;
    Miro::PositionIDL odoData;

    while(loop && !canceled)
    {
      Miro::RangeGroupEventIDL_var  pSonarEvent;
      Miro::PositionIDL odoData;
      //cout << "hier1" << endl;
      /*pSonarEvent = service->pRangeSensorImpl->getGroup(0);
      service->pOdometryImpl->setPosition(odoData);
      odoData = service->pOdometryImpl->getWaitPosition();
      k=100;
      //odoData = service->pOdometryImpl->getWaitPosition();
      cout << "Odo:__" << odoData << endl;
      */
      //service->connection.setSpeed(0);

      //service->connection.setStallId(stallId);
      while(loop)
      {
	  cout << endl
	       << "*****  Menu:  *****"<< endl
	       << "1 - motor on (L/R)! " << endl
	       << "2 - motor on " << endl
	       << "3 - Befehl!!!" << endl
	       << "8 - Odo status" << endl
	       << "4 - flooding test" << endl
	    /*
	       << "6 - set servo" << endl
	       << "7 - status (Sonar and Stall infos)" << endl
	       << "8 - Odo status" << endl
	       << "9 - set rotation velocity" << endl
	    */
	       << "0 - all motors off!!!" << endl
	       << "g - get vel" << endl
	       << "p - get pos" << endl
               //<< "t - test endlosschleife" << endl
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
	     service->pFaulhaber->connection.setSpeed(k, l);
	     break;
	   }

	 case '2' :
	   {
	     cout << "what speed? (mm/sec) " << endl;
	     cin >> k;
	     service->pFaulhaber->connection.setSpeed(k,k);break;
	     /*
	     cout << "how long?  (mmsec)" << endl;
	     cin >> i;
	     ace_time.msec(i);
	     cout << "what speed? (mm/sec)  " << endl;
	     cin >> k;
	     service->connection.setSpeed(k);
	     ACE_OS::sleep(ace_time);
	     service->connection.setSpeed(0);break;*/
	   }

	 case '3' :
	   {
	     cout << "Befehleingeben: " << endl;
	     cin >> eing;
	     service->pFaulhaber->connection.setBefehl(eing);
	     //service->connection.turn(k);
	     break;
	   }

	 case '4' :
	   {
	     int iter;
	     int timeout;
	     cout << "iterations:  " << endl;
	     cin >> iter;
	     cout << "timeout (ms) " << endl;
	     cin >> timeout;
	     floodingTest(iter, timeout);
	     break;
	   }

	 case '5' :                                 // muss ich noch testen
	   {
	     cout << "Enter Servo middle pules! (initial 129) " << endl;
	     cin >> k;
	     //service->connection.setServoMidPulse((unsigned short)k);
	     cout << "Servo auf null gestellt mit: " << k << endl;
	     break;
	   }

	 case '6' :
	   {
	     cout << "Servosetting:" << endl
		  << "  0: middle Setting " << endl
		  << "  +/- 90°" << endl;
	     cin >> k;
	     //service->connection.setServo(k);
	     break;
	   }

	 case '7' :
	   {
	     /*pSonarEvent = service->pRangeSensorImpl->getGroup(0);
	     for (i=0;i<=7;i++)
	       {  cout << "Sonar"  << i << ":__" << pSonarEvent->range[i] << endl; }
	     k= service->pStallImpl->getStalledWheels();
	     if (k==1) cout <<"only left wheel stalled! " << endl;
	     if (k==2) cout <<"only right wheel stalled! " << endl;
	     if (k==3) cout <<"left and right wheel stalled!!! " << endl;
	     cout << "Battery (volt):  " << service->pBatteryImpl->getVoltage() << endl;*/
	     break;
	   }

	 case '8' :
	   {
	     odoData = service->pOdometryImpl->getPosition();
	     cout << "Odo:__" << odoData << endl;break;
	   }

	 case '9' :
	   {
	     //cout << pOdometryImpl->getPosition() << endl;

	     //service->connection.setRotVel(k);break;
	     break;
	   }

	 case '0' :
	   {
	     service->pFaulhaber->connection.setSpeed(0,0);
	     //service->connection.setSpeed(0);
	     //service->connection.stop();
	     break;
	   }
	 case 'a' :
	   {

	     break;
	   }
	 case 't' :
	   {
             /*cout << "how long?  (msec)" << endl;
             cin >> i;
             ace_time.msec(i);

	     while(true)
	     {
		service->pFaulhaber->connection.setSpeed(100);
                ACE_OS::sleep(ace_time);
                service->pFaulhaber->connection.setSpeed(0);
                ACE_OS::sleep(ace_time);

             }*/
	     break;
	   }
	  case 'g' :
	   {
	     service->pFaulhaber->connection.getSpeed();
	     break;
	   }
	 case 'p' :
	   {
	     service->pFaulhaber->connection.getTicks();
	     break;
	   }
	 case 'X':
	 case 'x' :
	   loop = false;
	   break;
	 default:
	   cout << "gibts doch gar net!!!" << endl;
	 }//switch(c)
      }
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

  service->reactorTask.cancel();
  delete service;
  return 0;
}





