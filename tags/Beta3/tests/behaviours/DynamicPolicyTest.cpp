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
#include "DummyBehaviourD.h"
#include "DummyArbiter.h"

#include "miro/BehaviourRepository.h"
#include "miro/ArbiterRepository.h"
#include "miro/ActionPattern.h"
#include "miro/Policy.h"
#include "miro/Thread.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

class Thread : public Miro::Thread
{
public:
  Thread(Miro::Policy& _policy) :
    policy_(_policy)
  {}

  int
  svc() {
    while(!canceled()) {
      Miro::BehaviourParameters * const p  =
	policy_.getBehaviourParameters("Pattern0", "Behaviour0");
      DummyBehaviourParameters * const params = 
	dynamic_cast<DummyBehaviourParameters * const>(p);

      params->dummy++;

      policy_.setBehaviourParameters("Pattern0", "Behaviour0", params);

      ACE_OS::sleep(ACE_Time_Value(0, 10000));
    }

    return 0;
  }

protected:
  Miro::Policy& policy_;
};

int
main(int, char**)
{
  // The available behaviours
  DummyBehaviour behaviour0("Behaviour0");
  DummyBehaviour behaviour1("Behaviour1");
  DummyBehaviourB behaviourB("BehaviourB");
  DummyBehaviourD behaviourD;

  // The behaviour parameters
  DummyBehaviourParameters * parameters00 = behaviour0.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters01 = behaviour1.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters0B = behaviourB.getParametersInstance();
  parameters00->pace = ACE_Time_Value(0, 100000);
  parameters01->pace = ACE_Time_Value(0, 100000);
  parameters0B->pace = ACE_Time_Value(2);

  Miro::TimedBehaviourParameters * parameters10 = behaviour0.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters11 = behaviourD.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters1B = behaviourB.getParametersInstance();
  parameters10->pace = ACE_Time_Value(1);
  parameters11->pace = ACE_Time_Value(2);
  parameters1B->pace = ACE_Time_Value(2);

  DummyArbiter arbiter0("Arbiter0");

  Miro::BehaviourRepository * br = Miro::BehaviourRepository::instance();
  Miro::ArbiterRepository * ar = Miro::ArbiterRepository::instance();

  br->registerBehaviour(&behaviour0);
  br->registerBehaviour(&behaviour1);
  br->registerBehaviour(&behaviourB);

  ar->registerArbiter(&arbiter0);

  Miro::ActionPattern pattern0("Pattern0");
  Miro::ActionPattern pattern1("Pattern1");

  pattern0.arbiter(&arbiter0, arbiter0.getParametersInstance());
  pattern0.addBehaviour(&behaviour0, parameters00);
  pattern0.addBehaviour(&behaviour1, parameters01);
  pattern0.addBehaviour(&behaviourB, parameters0B);
  pattern0.addTransition("Transition", &pattern1);

  pattern1.arbiter(&arbiter0, arbiter0.getParametersInstance());
  pattern1.addBehaviour(&behaviourD, parameters11);
  pattern1.addBehaviour(&behaviour1, parameters10);
  pattern1.addBehaviour(&behaviourB, parameters1B);
  pattern1.addTransition("Transition", &pattern0);
  
  Miro::Policy policy;
  policy.registerActionPattern(&pattern0);
  policy.registerActionPattern(&pattern1);

  policy.setStartPattern(&pattern0);
  // force the policy to be valid
  policy.valid(true);

  std::cout << "Policy: " << endl 
	    << policy;

  std::cout << "Constructing dynmic policy thread." << endl;
  Thread thread(policy);

  std::cout << "Opening policy." << endl;
  policy.open();
  std::cout << "Opening thread." << endl;
  thread.detach(3);

  std::cout << "Entering event loop." << endl;
  ACE_Reactor::run_event_loop ();

  return 0;
}
