// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "sparrow/SparrowConnection2003.h"
#include "sparrow/SparrowConsumer2003.h"
#include "sparrow/Parameters.h"

#include "can/CanEventHandler.h"

#include "miro/ReactorTask.h"
#include "miro/Configuration.h"
#include "miro/TimeHelper.h"
#include "miro/Angle.h"
#include "miro/Log.h"
#include "miro/IO.h"

#include <iostream>
#include <string>

using namespace Miro;
using namespace std;

double normalizeAngle(double alpha)
{
  while (alpha > M_PI)
    alpha -= 2 * M_PI;
  while (alpha < -M_PI)
    alpha += 2 * M_PI;

  return alpha;
}

struct Service
{
  ReactorTask reactorTask;
  //Miro::OdometryImpl * odometryImpl;
  Sparrow::Consumer2003 * pSparrowConsumer;
  Can::EventHandler * pCanEventHandler;
  Sparrow::Connection2003 connection;

  Service();
};

Service::Service() :
  reactorTask(),
  //odometryImpl(new Miro::OdometryImpl(NULL)),
  pSparrowConsumer(new Sparrow::Consumer2003()),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer, Sparrow::Parameters::instance())),
  connection(reactorTask.reactor(), pCanEventHandler, pSparrowConsumer)
{
  pSparrowConsumer->registerInterfaces(&connection, NULL, NULL, NULL, NULL, NULL);
}

int main(int argc, char * argv[])
{
  Miro::Log::init(argc, argv);
  Miro::Configuration::init(argc, argv);

  // Parameters to be passed to the services
  Miro::RobotParameters * robotParams = Miro::RobotParameters::instance();
  Sparrow::Parameters * params = Sparrow::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument * config = Miro::Configuration::document();
  config->setSection("Robot");
  config->getParameters("Miro::RobotParameters", *robotParams);
  config->setSection("Sparrow");
  config->getParameters("Sparrow::Parameters", *params);

  MIRO_LOG_OSTR(LL_NOTICE,  "  sparrow paramters:\n" <<* params);

  // Initialize server daemon.
  Service service;

  MIRO_LOG(LL_NOTICE, "Initialized");

  unsigned short nMotorflood, counter;
  long sleepTime;
  int ventilatetime, kicktime;
  char c;
  double rot;
  bool loop = true;

  char motor_command[64];
  char motor_id[64];

  service.reactorTask.open(NULL);

  ACE_OS::sleep(ACE_Time_Value(2));

  try {
    while(loop) {

      cout << endl
	   << "Befehlsauswahl:" << endl

           << "  7 - kick" << endl
	   << "  f - motor flood" << endl
	   << "  m - send motor command" << endl
           << "  s - set servo" << endl
	   << "q - quit" << endl;
      cin >> c;
      cout << endl;

      switch (c) {

      case '7':      // kick
        cout << "ventilatetime (msec): " << flush;
	cin >> ventilatetime;
        cout << "kicktime (msec): " << flush;
	cin >> kicktime;
	service.connection.kick(ventilatetime, kicktime);
	break;
      case 'm':
      	cout << "hint : if nothing happens, try to use command 'en' (for both motors) for enabling motor controllers..." << endl;
      	cout << "which motor? --> (l)eft, (r)ight, (b)oth: " << flush;
	cin >> motor_id;
      	cout << "command: " << flush;
	cin >> motor_command;
	if(!strncmp(motor_id, "l", 1) || !strncmp(motor_id, "b", 1))
	  service.connection.writeWheel(motor_command, strlen(motor_command), 0);
	if(!strncmp(motor_id, "r", 1) || !strncmp(motor_id, "b", 1))
	  service.connection.writeWheel(motor_command, strlen(motor_command), 1);
      	break;


      case 's':      // set pan
	cout << "pan position (deg): " << flush;
	cin >> rot;
//	rot = deg2Rad(rot);
	service.connection.setPan(rot);
	break;

      case 'f':      //MotorFlood
	cout << "number of FloodPings: " << flush;
	cin >> nMotorflood;
	cout << "sleepTime betweenPings (microseconds): " << flush;
	cin >> sleepTime;
        for(counter = 0; counter < nMotorflood; counter++){
	   service.connection.writeFloodPing(counter);
	   ACE_OS::sleep(ACE_Time_Value(0, sleepTime));
	}
	break;
      case 'q':
	loop = false;
	break;
      default:
	cout << "unknown command: " << c << endl;
      }
    }
  }
  catch (const Miro::Exception & e) {
    cerr << "Miro exception thrown:" << e << endl;
    return 1;
  }

  service.reactorTask.cancel();
  return 0;
}

