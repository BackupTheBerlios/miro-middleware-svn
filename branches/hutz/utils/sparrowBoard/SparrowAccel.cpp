// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "sparrow/SparrowConnection.h"
#include "sparrow/SparrowParameters.h"
#include "sparrow/SparrowConsumer.h"
#include "sparrow/SparrowDevice.h"

#include "can/CanEventHandler.h"

#include "miro/ReactorTask.h"
#include "miro/Utils.h"

#include "miro/TimeHelper.h"

#include <iostream>

using namespace Miro;

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
  pSparrowConsumer(new Sparrow::Consumer(connection, NULL, NULL, NULL, NULL)),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer)),
  connection(reactorTask.reactor(), pCanEventHandler, pSparrowConsumer)
{

}

int main(int argc, char *argv[])
{
  char * fileName = NULL;
  bool write = false;
  int i = 1;

  Sparrow::Parameters * parameters = Sparrow::Parameters::instance();

  // Config file processing
  Miro::ConfigDocument config(argc, argv);
  config.setRobotType("Sparrow99");
  config.getParameters("sparrowBoard", *parameters);

#ifdef DEBUG
  cout << "  sparrow paramters:" << endl << *parameters << endl;
#endif

  if (argc > 1) {
    if (strcmp("-w", argv[i]) == 0) {
      write = true;
      ++i;
    }
  }

  if (argc > i)
    fileName = argv[i];

  // Initialize server daemon.
  Service service;

  cout << "initialized" << endl;

  service.reactorTask.open(NULL);

  ACE_OS::sleep(ACE_Time_Value(2));

  try {
    if (write) {
      cout << "Table entries: " << endl
	   << "  table1: " << parameters->table1.size() << endl
	   << "  table2: " << parameters->table2.size() << endl;

      int minimum = std::min(parameters->table1.size(),
			     parameters->table2.size());
      for (int j = 0; j < minimum; ++j)
	service.connection.setAccelValues(j, 
					  parameters->table1[j],
					  parameters->table2[j]);
    }

    cout << "receiving tables:" << endl;
    service.pSparrowConsumer->accelMutex.acquire();
    service.connection.getAccelValues();
    service.pSparrowConsumer->accelCond.wait();
    service.pSparrowConsumer->accelMutex.release();
    
    cout << "table 1" << endl;
    short * table1 = service.pSparrowConsumer->getTable1();
    for (int i = 0; i < Sparrow::ACCEL_TABLE_SIZE; ++i) {
      cout << table1[i] << " \t";
      if ( ((i+1) % 8) == 0)
	cout << endl;
    }

    cout << endl << endl << "table 2" << endl;

    short * table2 = service.pSparrowConsumer->getTable2();
    for (int i = 0; i < Sparrow::ACCEL_TABLE_SIZE; ++i) {
      cout << table2[i] << " \t";
      if ( ( (i+1) % 8) == 0)
	cout << endl;
    }

    if (fileName != NULL) {
      cout << "need to imlement writing file: " << fileName << endl;
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




  
