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

#include "../simple/BehavioursFactory.h"

#include "botsvcs/BAP/BehaviourEngineImpl.h"

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/StructuredPushSupplier.h"

#include <orbsvcs/CosNotifyCommC.h>

using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_var;

using Miro::Server;
using Miro::ReactorTask;

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
  Server server(argc, argv);
  ReactorTask * task = new ReactorTask(&server);

  if (argc < 2) {
    cout << "usage: " << argv[0] << "[ <POLICYFILE> ]" << endl
	 << "warning: No policy specified on startup." << endl;
  }

  try {
    SimpleBehavioursFactory behaviours(server, *task->reactor());
    behaviours.init();

    // Notification Service
    EventChannel_var ec = server.resolveName<EventChannel>("EventChannel");
    
    Miro::StructuredPushSupplier supplier_(ec.in(), server.namingContextName, false);
    
    // Init behaviour infrastructure
    Miro::BAP::BehaviourEngineImpl engineImpl(&supplier_);
    Miro::BehaviourEngine_var engineInterface = engineImpl._this();
    server.addToNameService(engineInterface.in(), "BehaviourEngine");

    // initialize ActionPatterns
    if (argc >= 2)
      engineImpl.loadPolicyFile(argv[1]);

    // start timed behaviour sceduler
    task->open(NULL);

    behaviours.connect();
    supplier_.connect();

    cout << "Loop forever handling events." << endl;
    server.run(5);
    cout << "Server stoped, exiting." << endl;
    server.detach(1);

    engineImpl.closePolicy();

    behaviours.disconnect();
    supplier_.disconnect();

    task->cancel();

  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }

  server.shutdown();
  server.wait();
  return 0;
}
