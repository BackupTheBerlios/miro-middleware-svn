// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "sparrow/SparrowConnection.h"
#include "sparrow/Parameters.h"
#include "sparrow/SparrowConsumer.h"

#include "can/CanEventHandler.h"

#include "miro/ReactorTask.h"
#include "miro/Utils.h"

#include "miro/TimeHelper.h"

#include <iostream>

using namespace Miro;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

struct Service
{
  ReactorTask reactorTask;
  Sparrow::Consumer * pSparrowConsumer;
  Can::EventHandler * pCanEventHandler;
  Sparrow::Connection connection;

  Service();
};

Service::Service() :
  reactorTask(),
  pSparrowConsumer(new Sparrow::Consumer(&connection, NULL, NULL, NULL, NULL)),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer, Sparrow::Parameters::instance())),
  connection(reactorTask.reactor(), pCanEventHandler, pSparrowConsumer)
{

}

int main(int argc, char * argv[])
{
  // Parameters to be passed to the services
  Sparrow::Parameters * parameters = Sparrow::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
  config->setSection("Sparrow99");
  config->getParameters("sparrowBoard", *parameters);
  delete config;

#ifdef DEBUG
    cout << "  sparrow paramters:" << endl << *parameters << endl;
#endif

  // Initialize server daemon.
  Service service;

  cout << "initialized" << endl;

  int time;

  service.reactorTask.open(NULL);

  ACE_OS::sleep(ACE_Time_Value(2));

  if (argc > 1)
    time = atoi(argv[1]);
  else {
    cerr << "usage: kickerTest <kick msec>" << endl;
    return 1;
  }
  
  try {
    int digital = 1;

    while (true) {
      Miro::Guard guard(service.pSparrowConsumer->digitalMutex);
      service.pSparrowConsumer->digitalCond.wait();

      cout << "button" << endl;

      if ((service.pSparrowConsumer->digital[1] & 0x01) && digital == 0) {

	cout << "time (msec): " << time << endl;
	service.connection.kick(time);
      }
      digital = service.pSparrowConsumer->digital[1] & (0x01);

    }
  }
  catch (const Miro::Exception & e) {
    cerr << "Miro exception thrown:" << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }

  service.reactorTask.cancel();
  return 0;
}

