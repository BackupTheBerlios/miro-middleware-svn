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
#include "../lib/DummyArbiter.h"

#include "botsvcs/BAP/ActionPattern.h"
#include "botsvcs/BAP/Policy.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

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
  Miro::BAP::TimedBehaviourParameters * parameters00 = behaviour0.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters0B = behaviourB.getParametersInstance();
  parameters00->pace = 1;
  parameters0B->pace = 2;

  Miro::BAP::TimedBehaviourParameters * parameters10 = behaviour0.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters11 = behaviour1.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters1B = behaviourB.getParametersInstance();
  parameters10->pace = 1;
  parameters11->pace = 2;
  parameters1B->pace = 2;

  Miro::BAP::TimedBehaviourParameters * parameters22 = behaviour0.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters23 = behaviour1.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters2B = behaviourB.getParametersInstance();
  parameters22->pace = 1;
  parameters23->pace = 1;
  parameters1B->pace = ACE_Time_Value(1,500);

  Miro::BAP::TimedBehaviourParameters * parameters30 = behaviour0.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters31 = behaviour1.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters32 = behaviour2.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters33 = behaviour3.getParametersInstance();
  Miro::BAP::TimedBehaviourParameters * parameters3B = behaviourB.getParametersInstance();
  parameters30->pace = 1;
  parameters31->pace = 2;
  parameters32->pace = 3;
  parameters33->pace = 4;
  parameters1B->pace = 4;

  DummyArbiter arbiter0("Arbiter0");
  DummyArbiter arbiter1("Arbiter1");

  Miro::BAP::ActionPattern pattern0("Pattern0");
  Miro::BAP::ActionPattern pattern1("Pattern1");
  Miro::BAP::ActionPattern pattern2("Pattern2");
  Miro::BAP::ActionPattern pattern3("Pattern3");

  pattern0.arbiter(&arbiter0, arbiter0.getParametersInstance());
  pattern0.addBehaviour(&behaviour0, parameters00);
  pattern0.addBehaviour(&behaviourB, parameters0B);
  pattern0.addTransition("Transition", &pattern1);

  pattern1.arbiter(&arbiter0, arbiter0.getParametersInstance());
  pattern1.addBehaviour(&behaviour1, parameters10);
  pattern1.addBehaviour(&behaviour0, parameters11);
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
  
  Miro::BAP::Policy policy;
  policy.registerPattern(&pattern0);
  policy.registerPattern(&pattern1);
  policy.registerPattern(&pattern2);
  policy.registerPattern(&pattern3);

  policy.setStartPattern(&pattern0);
  // force the policy to be valid
  policy.valid(true);

  std::cout << "Policy: " << endl 
	    << policy;

  policy.open(NULL, std::string());

  ACE_Reactor::run_event_loop ();

  return 0;
}
