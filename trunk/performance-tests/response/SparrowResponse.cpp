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
#include <ace/Stats.h>
#include <ace/Sample_History.h>
#include <ace/High_Res_Timer.h>

#include "sparrow/SparrowConnection.h"
#include "sparrow/SparrowConsumer.h"
#include "sparrow/Parameters.h"

#include "can/CanEventHandler.h"

#include "miro/ReactorTask.h"
#include "miro/IO.h"
#include "miro/Utils.h"

#include <iostream>
#include <cmath>

#include <stdlib.h>
#include <sys/time.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;
using std::vector;
using namespace Miro;

struct Service
{
  ReactorTask reactorTask;
  Miro::OdometryImpl * odometryImpl;
  Sparrow::Consumer * pSparrowConsumer;
  Can::EventHandler * pCanEventHandler;
  Sparrow::Connection connection;

  Service();
};

Service::Service() :
  reactorTask(),
  odometryImpl(new Miro::OdometryImpl(NULL)),
  pSparrowConsumer(new Sparrow::Consumer(&connection, odometryImpl, NULL, NULL, NULL)),
  pCanEventHandler(new Can::EventHandler(pSparrowConsumer, Sparrow::Parameters::instance())),
  connection(reactorTask.reactor(), pCanEventHandler, pSparrowConsumer)
{

}

double rand1()
{
  return (double)rand() / (RAND_MAX + 1.0);
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

  Miro::PositionIDL startPos, current;

  service.reactorTask.open(NULL);

  int ticks = 0;
  int niterations = 10;

  if (argc > 1)
    niterations = ACE_OS::atoi(argv[1]);

  cout << "start test" << endl;
  try {
    ACE_Sample_History history (niterations);
    
    ACE_hrtime_t test_start = ACE_OS::gethrtime ();
    for (int i = 0; i < niterations; ++i) {

      service.connection.setPower(0, 0);
      
      ACE_OS::sleep(ACE_Time_Value(1, (int)floor(rand1() * 100000.)));
      startPos = service.odometryImpl->getPosition();
      cout << "start position:" << startPos << endl;

      ACE_hrtime_t start = ACE_OS::gethrtime ();

      service.connection.setPower(10000, 10000);
      do {
	current = service.odometryImpl->getWaitPosition();
	++ticks;
      }
      while (startPos.point.x == current.point.x &&
	     startPos.point.y == current.point.y &&
	     startPos.heading == current.heading);
      
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }
    service.connection.setPower(0, 0);

    ACE_hrtime_t test_end = ACE_OS::gethrtime ();

    ACE_DEBUG ((LM_DEBUG, "test finished\n"));
    
    ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
    ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
    ACE_DEBUG ((LM_DEBUG, "done\n"));

    history.dump_samples ("HISTORY", gsf);

    ACE_Basic_Stats stats;
    history.collect_basic_stats (stats);
    stats.dump_results ("Total", gsf);
    
    ACE_Throughput_Stats::dump_throughput ("Total", gsf,
					   test_end - test_start,
					   stats.samples_count ());

    // data evaluation
    cout << "ticks: " << ticks << endl;
    cout << "end position: " << current << endl;
  }
  catch (const Miro::Exception & e) {
    cerr << "Miro exception thrown:" << e << endl;
    return 1;
  }

  service.reactorTask.cancel();
  return 0;
}

