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


#include <ace/Stats.h>
#include <ace/Sample_History.h>
#include <ace/High_Res_Timer.h>

#include "idl/OdometryC.h"
#include "idl/SparrowMotionC.h"

#include "miro/Server.h"
#include "miro/IO.h"
#include "miro/Utils.h"


#include <vector>
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
using Miro::Server;
using Miro::SparrowMotion;
using Miro::SparrowMotion_var;
using Miro::Odometry;
using Miro::Odometry_var;
using Miro::PositionIDL;
using Miro::VelocityIDL;

double rand1()
{
  return (double)rand() / (RAND_MAX + 1.0);
}

int main(int argc, char * argv[])
{
  Server server(argc, argv);

  SparrowMotion_var motion = server.resolveName<SparrowMotion>("Motion");
  Odometry_var odometry = server.resolveName<Odometry>("Odometry");

  PositionIDL startPos, current;

  int ticks = 0;
  int niterations = 10;

  if (argc > 1)
    niterations = ACE_OS::atoi(argv[1]);

  cout << "start test" << endl;
  try {
    ACE_Sample_History history (niterations);
    
    ACE_hrtime_t test_start = ACE_OS::gethrtime ();
    for (int i = 0; i < niterations; ++i) {

      motion->setLRPower(0, 0);
      
      ACE_OS::sleep(ACE_Time_Value(1, (int)floor(rand1() * 100000.)));
      startPos = odometry->getPosition();
      cout << "start position:" << startPos << endl;
      
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      motion->setLRPower(10000, 10000);
      do {
	current = odometry->getWaitPosition();
	++ticks;
      }
      while (startPos.point.x == current.point.x &&
	     startPos.point.y == current.point.y &&
	     startPos.heading == current.heading);
  
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      history.sample (now - start);
    }
    motion->setLRPower(0, 0);

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

  return 0;
}

