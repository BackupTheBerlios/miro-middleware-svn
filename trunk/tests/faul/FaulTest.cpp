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

#include "faulTty/FaulTtyEventHandler.h"

#include "miro/ReactorTask.h"
#include "miro/IO.h"
#include "miro/Utils.h"
#include "miro/TimeHelper.h"
#include "miro/RangeSensorImpl.h"
#include "miro/OdometryImpl.h"
#include "miro/BatteryImpl.h"
#include "miro/Exception.h"

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
  FaulMotor::TimerEventHandler * pTimerEventHandler;
  FaulMotor::Connection connection;

  Service();
};


Service::Service() :
  reactorTask(),
  pOdometryImpl(new Miro::OdometryImpl(NULL)),
  pConsumer(new FaulMotor::Consumer(pOdometryImpl)),
  pTimerEventHandler(new FaulMotor::TimerEventHandler(connection)),
  connection(reactorTask.reactor(), pConsumer)
{}



int main(int argc, char* argv[])
{
  // Parameters to be passed to the services
  FaulMotor::Parameters * pParams = FaulMotor::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
  config->setSection("Faulhaber");
  config->getParameters("FaulMotor", *pParams);
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
  int stallId;


  // register Signal handler for Ctr+C
  sigs.sig_add(SIGINT);
  sigs.sig_add(SIGTERM);

  if (service.reactorTask.reactor()->register_handler(sigs, &event) == -1) {
    throw Miro::ACE_Exception(errno, "failed to register signal handler");
  }


  // adding a handler for odometry polling

  ACE_Time_Value tv(0, 500000);
  service.reactorTask.reactor()->schedule_timer(service.pTimerEventHandler, NULL, tv ,tv);


  // adding a handler for stall detection
  /*
    ACE_Time_Value t0(0,0);
    stallId = service.reactorTask.reactor()->schedule_Timer(service.pTimerEventHandler, NULL, t0, t0);
  */
  // service.connection.setStallId(stallId);

  service.reactorTask.open(NULL);

  try
  {
    Miro::RangeGroupEventIDL_var  pSonarEvent;
    Miro::PositionIDL odoData;

    while(loop && !canceled)
    {
      cout << endl
	   << "*****  Menu:  *****"<< endl
	   << "1 - motor on (L/R)! " << endl
	   << "2 - motor on " << endl
	   << "3 - Befehl!!!" << endl
	   << "8 - Odo status" << endl
	/*<< "4 - turn and drive" << endl
	  << "6 - set servo" << endl
	  << "7 - status (Sonar and Stall infos)" << endl
	  << "8 - Odo status" << endl
	  << "9 - set rotation velocity" << endl*/
	   << "0 - all motors off!!!" << endl
	   << "g - get vel" << endl
	   << "p - get pos" << endl
	   << "t - test endlosschleife" << endl
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
	  service.connection.setSpeed(k, k);break;
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
	  break;
	}

      case '6' :
	{
	  break;
	}

      case '7' :
	{
	  break;
	}

      case '8' :
	{
	  odoData = service.pOdometryImpl->getPosition();
	  cout << "Odo: " << odoData << endl;
	  break;
	}

      case '9' :
	{
	  break;
	}

      case '0' :
	{
	  service.connection.setSpeed(0, 0);
	  break;
	}
      case 'a' :
	{

	  break;
	}
      case 't' :
	{
	  cout << "how long?  (msec)" << endl;
	  cin >> i;
	  ace_time.msec(i);

	  while(!canceled) {
	    service.connection.setSpeed(300, 300);
	    ACE_OS::sleep(ace_time);
	    service.connection.setSpeed(0, 0);
	    ACE_OS::sleep(ace_time);
                
	  }
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
      case 'x' : 
	loop = false;
	break;
      default: 
	cout << "gibts doch gar net!!!" << endl;

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

  service.reactorTask.cancel();
  return 0;
}





