// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "WallFollow.h"
#include "RangeSensorAvoid.h"
#include "TactileStop.h"
#include "Straight.h"
#include "Wander.h"
#include "OnButton.h"

#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/MotionArbiter.h"
#include "miro/BehaviourRepository.h"
#include "miro/ArbiterRepository.h"
#include "miro/BehaviourEngineImpl.h"

#include "miro/RangeSensorC.h"
#include "miro/MotionC.h"
#include "miro/OdometryC.h"


#include "miro/StructuredPushSupplier.h"

#include <orbsvcs/CosNotifyCommC.h>

using CosNotifyChannelAdmin::EventChannel;
using CosNotifyChannelAdmin::EventChannel_var;

using Miro::RangeSensor;
using Miro::RangeSensor_var;
using Miro::Motion;
using Miro::Motion_var;
using Miro::Odometry;
using Miro::Odometry_var;

using Miro::Server;
using Miro::ReactorTask;
using Miro::BehaviourRepository;
using Miro::ArbiterRepository;
using Miro::MotionArbiter;
using Miro::Policy;

using std::cout;
using std::cerr;

int main(int argc, char *argv[])
{
  Server server(argc, argv);
  ReactorTask * task = new ReactorTask(&server);

  if (argc < 2) {
    cout << "usage: behaviours <BEHAVIOURFILE>" << endl;
    return 1;
  }

  try {
    // Notification Service
    cout << "Resolving event channel." << endl;
    EventChannel_var ec = server.resolveName<EventChannel>("EventChannel");
    
    Miro::StructuredPushSupplier supplier(ec.in(), server.namingContextName);
    
    // optain references
    cout << "Resolving sensor and actuator services." << endl;
    Motion_var motion = server.resolveName<Motion>("Motion");

    // construct all available behaviours
    cout << "Constructing Behaviours and Arbiters." << endl;
    TactileStop tactileStop(motion.in(), ec.in(), server.namingContextName);
    RangeSensorAvoid avoid1(server, ec.in(), "AvoidOne", server.namingContextName);
    RangeSensorAvoid avoid2(server, ec.in(), "AvoidTwo", server.namingContextName);
    WallFollow wallFollow(server, ec.in(), "WallFollow", server.namingContextName, &supplier);
    Straight straight(*task->reactor());
    Wander wander(*task->reactor());
    OnButton onButton(ec.in(), server.namingContextName);

    MotionArbiter ma(motion.in(), &supplier);
  
    // polpulating the repositories
    BehaviourRepository * bf = BehaviourRepository::instance();
    ArbiterRepository * af = ArbiterRepository::instance();

    bf->registerBehaviour(&onButton);
    bf->registerBehaviour(&tactileStop);
    bf->registerBehaviour(&avoid1);
    bf->registerBehaviour(&avoid2);
    bf->registerBehaviour(&wallFollow);
    bf->registerBehaviour(&straight);
    bf->registerBehaviour(&wander);

    af->registerArbiter(&ma);

    // Init behaviour infrastructure
    Miro::BehaviourEngineImpl engineImpl(&supplier);
    Miro::BehaviourEngine_var engineInterface = engineImpl._this();
    server.addToNameService(engineInterface.in(), "BehaviourEngine");

    // initialize ActionPatterns
    engineImpl.loadPolicyFile(argv[1]);

    // start timed behaviour sceduler
    task->open(NULL);

    cout << "Loop forever handling events." << endl;
    server.run(5);

    cout << "Server stoped, exiting." << endl;

    engineImpl.closePolicy();

    supplier.disconnect();

    tactileStop.disconnect();
    avoid1.disconnect();;
    avoid2.disconnect();
    wallFollow.disconnect();
    onButton.disconnect();

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
