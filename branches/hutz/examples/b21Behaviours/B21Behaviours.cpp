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

#include <miro/ReactorTask.h>
#include <miro/Policy.h>

#include "BehaviourEngine.h"

using Miro::Server;
using Miro::ReactorTask;
using Miro::Policy;

int main(int argc, char *argv[])
{
  Server server(argc, argv);
  ReactorTask * task = new ReactorTask(&server);

  if (argc < 2) {
    cout << "usage: b21Behaviours <BEHAVIOURFILE>" << endl;
    return 1;
  }

  try {
    // construct all available behaviours
    BehaviourEngine engine(server, *task->reactor());

    // construct action patterns
    Policy policy(argv[1]);
    cout << policy << endl;

    // start timed behaviour sceduler
    task->open(NULL);

    cout << "open policy" << endl;
    policy.open();
    
    cout << "Loop forever handling events." << endl;
    server.run(5);

    cout << "Server stoped, exiting." << endl;

    policy.close();
    task->cancel();
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception!" << endl;
    return 1;
  }
  return 0;
}
