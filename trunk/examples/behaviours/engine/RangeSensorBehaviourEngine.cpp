// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "../simple/BehavioursFactory.h"
#include "../rangeSensor/BehavioursFactory.h"

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/BehaviourEngineImpl.h"
#include "miro/StructuredPushSupplier.h"


#include <orbsvcs/CosNotifyCommC.h>

using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_var;

using Miro::Server;
using Miro::ReactorTask;

using std::cout;
using std::cerr;

int main(int argc, char *argv[])
{
  Server server(argc, argv);
  ReactorTask * task = new ReactorTask(&server);

  if (argc < 2) {
    cout << "usage: " << argv[0] << " <BEHAVIOURFILE>" << endl;
    return 1;
  }

  try {
    SimpleBehavioursFactory behaviours1(server, *task->reactor());
    RangeSensorBehavioursFactory behaviours2(server, *task->reactor());

    // Notification Service
    EventChannel_var ec = server.resolveName<EventChannel>("EventChannel");
    
    Miro::StructuredPushSupplier supplier_(ec.in(), server.namingContextName, false);
    
    // Init behaviour infrastructure
    Miro::BehaviourEngineImpl engineImpl(&supplier_);
    Miro::BehaviourEngine_var engineInterface = engineImpl._this();
    server.addToNameService(engineInterface.in(), "BehaviourEngine");

    // initialize ActionPatterns
    engineImpl.loadPolicyFile(argv[1]);

    // start timed behaviour sceduler
    task->open(NULL);

    behaviours1.open();
    behaviours2.open();
    supplier_.connect();

    cout << "Loop forever handling events." << endl;
    server.run(5);

    cout << "Server stoped, exiting." << endl;

    engineImpl.closePolicy();

    behaviours2.close();
    behaviours1.close();
    supplier_.disconnect();

    task->cancel();
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
