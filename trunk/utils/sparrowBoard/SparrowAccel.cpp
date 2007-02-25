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
#include "sparrow/SparrowConnection.h"
#include "sparrow/SparrowConsumer.h"
#include "sparrow/SparrowDevice.h"
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
  config.setSection("Sparrow99");
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
#ifdef WE_USE_THIS_FEATURE_AGAIN
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
#endif
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




  
