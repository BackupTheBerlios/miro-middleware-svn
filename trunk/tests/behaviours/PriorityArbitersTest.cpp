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
#include "miro/Policy.h"


int
main(int, char**)
{
  // The available behaviours
  DummyBehaviour behaviour0("Behaviour0");
  DummyBehaviour behaviour1("Behaviour1");
  DummyBehaviour behaviour2("Behaviour2");
  DummyBehaviour behaviour3("Behaviour3");
  DummyBehaviourB behaviourB("BehaviourB");

  // The behaviour parameters
  Miro::TimedBehaviourParameters * parameters00 = behaviour0.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters0B = behaviourB.getParametersInstance();
  parameters00->pace = ACE_Time_Value(1);
  parameters0B->pace = ACE_Time_Value(2);

  Miro::TimedBehaviourParameters * parameters10 = behaviour0.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters11 = behaviour1.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters1B = behaviourB.getParametersInstance();
  parameters10->pace = ACE_Time_Value(1);
  parameters11->pace = ACE_Time_Value(2);
  parameters1B->pace = ACE_Time_Value(2);

  Miro::TimedBehaviourParameters * parameters22 = behaviour0.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters23 = behaviour1.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters2B = behaviourB.getParametersInstance();
  parameters22->pace = ACE_Time_Value(1);
  parameters23->pace = ACE_Time_Value(1);
  parameters1B->pace = ACE_Time_Value(1, 500000);

  Miro::TimedBehaviourParameters * parameters30 = behaviour0.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters31 = behaviour1.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters32 = behaviour2.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters33 = behaviour3.getParametersInstance();
  Miro::TimedBehaviourParameters * parameters3B = behaviourB.getParametersInstance();
  parameters30->pace = ACE_Time_Value(1);
  parameters31->pace = ACE_Time_Value(2);
  parameters32->pace = ACE_Time_Value(3);
  parameters33->pace = ACE_Time_Value(4);
  parameters1B->pace = ACE_Time_Value(4);

  DummyArbiter arbiter0("Arbiter0");
  DummyArbiter arbiter1("Arbiter1");

  Miro::BehaviourRepository * br = Miro::BehaviourRepository::instance();
  Miro::ArbiterRepository * ar = Miro::ArbiterRepository::instance();

  br->registerBehaviour(&behaviour0);
  br->registerBehaviour(&behaviour1);
  br->registerBehaviour(&behaviour2);
  br->registerBehaviour(&behaviour3);
  br->registerBehaviour(&behaviourB);

  ar->registerArbiter(&arbiter0);
  ar->registerArbiter(&arbiter1);

  Miro::ActionPattern pattern0("Pattern0");
  Miro::ActionPattern pattern1("Pattern1");
  Miro::ActionPattern pattern2("Pattern2");
  Miro::ActionPattern pattern3("Pattern3");

  pattern0.arbiter(&arbiter0, arbiter0.getParametersInstance());
  pattern0.addBehaviour(&behaviour0, parameters00);
  pattern0.addBehaviour(&behaviourB, parameters0B);
  pattern0.addTransition("Transition", &pattern1);

  pattern1.arbiter(&arbiter0, arbiter0.getParametersInstance());
  pattern1.addBehaviour(&behaviour1, parameters10);
  pattern1.addBehaviour(&behaviour0, parameters10);
  pattern1.addBehaviour(&behaviourB, parameters1B);
  pattern1.addTransition("Transition", &pattern2);

  pattern2.arbiter(&arbiter1, arbiter1.getParametersInstance());
  pattern2.addBehaviour(&behaviour2, parameters22);
  pattern2.addBehaviour(&behaviour3, parameters23);
  pattern2.addBehaviour(&behaviourB, parameters2B);
  pattern2.addTransition("Transition", &pattern3);

  pattern3.arbiter(&arbiter1, arbiter1.getParametersInstance());
  pattern3.addBehaviour(&behaviour3, parameters33);
  pattern3.addBehaviour(&behaviour2, parameters32);
  pattern3.addBehaviour(&behaviour1, parameters31);
  pattern3.addBehaviour(&behaviour0, parameters30);
  pattern3.addBehaviour(&behaviourB, parameters3B);
  pattern3.addTransition("Transition", &pattern1);
  
  Miro::Policy policy;
  policy.registerActionPattern(&pattern0);
  policy.registerActionPattern(&pattern1);
  policy.registerActionPattern(&pattern2);
  policy.registerActionPattern(&pattern3);

  policy.setStartPattern(&pattern0);

  cout << "Policy: " << endl 
       << policy;

  policy.open();

  ACE_Reactor::run_event_loop ();

  return 0;
}
