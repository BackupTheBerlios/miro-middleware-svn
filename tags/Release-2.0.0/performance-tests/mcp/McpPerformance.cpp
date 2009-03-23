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
#include "McpPerformance.h"

#include "miro/Exception.h"
#include "miro/Utils.h"
#include "miro/TimeHelper.h"

#include "base/BaseImpl.h"
#include "base/Parameters.h"

#include <iostream>

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

McpPerformance::McpPerformance(int& , char **) :
  reactorTask(),
  b21Motion(reactorTask.reactor(), NULL)
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
  Base::Parameters * parameters = Base::Parameters::instance();

  try {
    // Config file processing
    Miro::ConfigDocument * config = new Miro::ConfigDocument(argc, argv);
    config->setSection("B21");
    config->getParameters("base", *parameters);
    delete config;

#ifdef DEBUG
    cout << "  base parameters:" << endl << *parameters << endl;
#endif

    DBG(cout << "Initialize mcp connection." << endl);
    McpPerformance mcpPerformance(argc, argv);

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

