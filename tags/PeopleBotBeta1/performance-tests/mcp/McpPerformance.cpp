// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "McpPerformance.h"

#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/TimeHelper.h"

#include "base/BaseImpl.h"
#include "base/BaseParameters.h"

#include <iostream>

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cerr;

McpPerformance::McpPerformance(int& , char **, 
			       const Base::Parameters& _parameters) :
  reactorTask(),
  b21Motion(reactorTask.reactor(), NULL, _parameters)
{
  // start the asychronous consumer listening for the hardware
  reactorTask.open(0);

  //  b21Motion.setStatusReportPeriod(26); // 1/10 sec

  DBG(cout << "McpPerformance initialized.." << endl);
}

McpPerformance::~McpPerformance()
{
  DBG(cout << "Destructing McpPerformance." << endl);

  reactorTask.cancel();
}

int
main(int argc, char *argv[])
{
  int rc = 0;

  // Parameters to be passed to the services
  Base::Parameters parameters;

  try {
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setRobotType("B21");
    config->getParameters("base", parameters);
    delete config;

#ifdef DEBUG
    cout << "  base parameters:" << endl << parameters << endl;
#endif

    DBG(cout << "Initialize mcp connection." << endl);
    McpPerformance mcpPerformance(argc, argv, parameters);

    int messages = 300;
    
    if (argc >=2)
      messages = atoi(argv[1]);

    cout << "Stressing Mcp by sending " << messages << " commands:" << endl;

    ACE_Time_Value start = ACE_OS::gettimeofday();
    cout << "Start time: " << start << endl;

    for (int i = messages / 2; i < 0; --i) {
      mcpPerformance.b21Motion.setRotateVelocity(M_PI/6);
      mcpPerformance.b21Motion.setRotateVelocity(M_PI/12);
    }
    ACE_Time_Value stop = ACE_OS::gettimeofday();

    cout << "Ending performance test." << endl;
    cout << "End time: " << stop << endl;
    cout << "Elapsed time: " << stop - start << endl;
    double msec = (stop-start).msec();
    cout << "Processing time per Messages: " 
	 << (msec / 1000. / messages) << endl;
  }
  catch (const Miro::CException& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
  }
  catch (const Miro::Exception& e) {
    cerr << "Miro exception: " << e << endl;
    rc = 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    rc = 1;
  }
  return rc;
}

