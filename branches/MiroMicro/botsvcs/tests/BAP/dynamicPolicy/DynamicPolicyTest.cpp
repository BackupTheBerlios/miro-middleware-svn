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
#include "../lib/DummyBehaviour.h"
#include "../lib/DummyBehaviourB.h"
#include "../lib/DummyBehaviourD.h"
#include "../lib/DummyArbiter.h"

#include "botsvcs/BAP/BehaviourRepository.h"
#include "botsvcs/BAP/ArbiterRepository.h"
#include "botsvcs/BAP/ActionPattern.h"
#include "botsvcs/BAP/Policy.h"

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
  Thread(Miro::BAP::Policy& _policy) :
    policy_(_policy)
  {}

  int
  svc() {
    while(!canceled()) {
      Miro::BAP::BehaviourParameters * const p  =
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
  Miro::BAP::Policy& policy_;
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
  Miro::BAP::TimedBehaviourParameters * parameters01 = behaviour1.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters0B = behaviourB.getParametersInstance();
  parameters00->pace = ACE_Time_Value(0, 100000);
  parameters01->pace = ACE_Time_Value(0, 100000);
  parameters0B->pace = ACE_Time_Value(2);

  Miro::BAP::TimedBehaviourParameters * parameters10 = behaviour0.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters11 = behaviourD.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters1B = behaviourB.getParametersInstance();
  parameters10->pace = ACE_Time_Value(1);
  parameters11->pace = ACE_Time_Value(2);
  parameters1B->pace = ACE_Time_Value(2);

  DummyArbiter arbiter0("Arbiter0");

//   Miro::BAP::BehaviourRepository * br = Miro::BAP::BehaviourRepository::instance();
//   Miro::BAP::ArbiterRepository * ar = Miro::BAP::ArbiterRepository::instance();

//   br->add(behaviour0.getName(), &behaviour0);
//   br->add(behaviour1.getName(), &behaviour1);
//   br->add(behaviourB.getName(), &behaviourB);

//   ar->add(arbiter0.getName(), &arbiter0);

  Miro::BAP::ActionPattern pattern0("Pattern0");
  Miro::BAP::ActionPattern pattern1("Pattern1");

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
  
  Miro::BAP::Policy policy;
  policy.registerPattern(&pattern0);
  policy.registerPattern(&pattern1);

  policy.setStartPattern(&pattern0);
  // force the policy to be valid
  policy.valid(true);

  std::cout << "Policy: " << endl 
	    << policy;

  std::cout << "Constructing dynmic policy thread." << endl;
  Thread thread(policy);

  std::cout << "Opening policy." << endl;
  policy.open(NULL, std::string());
  std::cout << "Opening thread." << endl;
  thread.detach(3);

  std::cout << "Entering event loop." << endl;
  ACE_Reactor::run_event_loop ();

  return 0;
}
