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

#include "sparrow/SparrowConnection.h"
#include "sparrow/SparrowConsumer.h"
#include "sparrow/Parameters.h"

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
  Sparrow::Connection sparrowConnection;

  Service();
};

Service::Service() :
  reactorTask(),
  pSparrowConsumer(new Sparrow::Consumer(&sparrowConnection, NULL, NULL, NULL, NULL)),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer, Sparrow::Parameters::instance())),
  sparrowConnection(reactorTask.reactor(), pCanEventHandler, pSparrowConsumer)
{

}

int main(int argc, char *argv[])
{
  Sparrow::Parameters * parameters = Sparrow::Parameters::instance();
  // Config file processing
  Miro::ConfigDocument config(argc, argv);
  config.setSection("Sparrow");
  config.getParameters("Sparrow::Parameters", *parameters);

#ifdef DEBUG
  cout << "  sparrow paramters:" << endl << *parameters << endl;
#endif


  int messages = 300;
  
  if (argc > 1)
    messages = atoi(argv[1]);

  if (argc > 2) {
    int timeOut = atoi(argv[2]);
    
    cout << "setting can time out to " << timeOut << " micro sec" << endl;
    Can::Connection::canTimeOut.usec(timeOut);
  }

  // Initialize server daemon.
  Service service;

  service.reactorTask.open(NULL);

  ACE_OS::sleep(ACE_Time_Value(2));

  try {
    cout << "Current counter: " << flush;
    service.sparrowConnection.testResult();
    ACE_OS::sleep(ACE_Time_Value(2));
    cout << endl
	 << "Sending " << messages << "messages" << endl;

    ACE_Time_Value start = ACE_OS::gettimeofday();
    for (int i = 0; i < messages; ++i) {
      service.sparrowConnection.testAdd();
      cout << "." << flush;
    }
    ACE_Time_Value stop = ACE_OS::gettimeofday();
    cout << endl;

    cout << "New counter: " << flush;
    service.sparrowConnection.testResult();
    ACE_OS::sleep(ACE_Time_Value(2));
    cout << endl;

    cout << "Waiting for thread to cancel." << endl;
    service.reactorTask.cancel();

    cout << "Elapsed time: " << stop - start << endl;
    double msec = (stop-start).msec();
    cout << "Processing time per Messages: " 
	 << (msec / 1000. / messages) << endl;

  }
  catch (const Miro::Exception & e) {
    cerr << "Miro exception thrown:" << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }
  return 0;
}




