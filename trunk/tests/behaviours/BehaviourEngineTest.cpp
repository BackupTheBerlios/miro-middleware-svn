// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "DummyBehaviour.h"
#include "DummyBehaviourB.h"
#include "DummyArbiter.h"

#include "miro/BehaviourRepository.h"
#include "miro/ArbiterRepository.h"
#include "miro/ActionPattern.h"
#include "miro/BehaviourEngineImpl.h"
#include "miro/Server.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int
main(int argc, char* argv[])
{
  Miro::Server server(argc, argv);

  try {
    // the available behaviours
    DummyBehaviour behaviour0("Behaviour0");
    DummyBehaviour behaviour1("Behaviour1");
    DummyBehaviour behaviour2("Behaviour2");
    DummyBehaviour behaviour3("Behaviour3");
    DummyBehaviourB behaviourB("BehaviourB");
    
    // the available arbiters
    DummyArbiter arbiter0("Arbiter0");
    DummyArbiter arbiter1("Arbiter1");
    
    // the repositories
    Miro::BehaviourRepository * br = Miro::BehaviourRepository::instance();
    Miro::ArbiterRepository * ar = Miro::ArbiterRepository::instance();
    
    // registering the behaviours
    br->registerBehaviour(&behaviour0);
    br->registerBehaviour(&behaviour1);
    br->registerBehaviour(&behaviour2);
    br->registerBehaviour(&behaviour3);
    br->registerBehaviour(&behaviourB);
    
    // registering the arbiters
    ar->registerArbiter(&arbiter0);
    ar->registerArbiter(&arbiter1);

    Miro::BehaviourEngineImpl * engineImpl = new Miro::BehaviourEngineImpl();
    Miro::BehaviourEngine_var engine = engineImpl->_this();
    server.addToNameService(engine.in(), "BehaviourEngine");

    std::cout << "loop forever handling events" << endl;
    server.run();

  }
  catch (const Miro::Exception& e) {
    std::cout << endl
	      << "Miro::Exception:" << endl
	      << e << endl;
  }

  return 0;
}
