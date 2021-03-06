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

#include "miro/BehaviourEngineC.h"
#include "miro/BehaviourRepository.h"
#include "miro/ArbiterRepository.h"
#include "miro/ActionPattern.h"
#include "miro/Policy.h"


int
main(int argc, char* argv[])
{
  // command line argument parsing
  if (argc <= 1) {
    std::cout << "usage: " << argv[0] << " <policy file>" << endl;
    return 1;
  }

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
    
    // parsing the policy
    Miro::Policy policy(argv[1]);

    cout << "Policy: " << endl 
	 << policy;
  }
  catch(const Miro::BehaviourEngine::EMalformedPolicy& e) {
    std::cerr << "Error parsing policy: " << e.what << endl;
  }
  catch(const Miro::BehaviourEngine::EMalformedXML& ) {
    std::cerr << "Malformed XML in file: " << argv[1] << endl;
  }
  catch(const Miro::BehaviourEngine::ENoPolicy& ) {
    std::cerr << "No policy loaded" << endl;
  }
  catch(const Miro::BehaviourEngine::EFile& ) {
    std::cerr << "Error loading file: " << argv[1] << endl;
  }
  catch (const Miro::Exception& e) {
    std::cout << endl
	      << "Miro::Exception:" << endl
	      << e << endl;
  }

  return 0;
}
