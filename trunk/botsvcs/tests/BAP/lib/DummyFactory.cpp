// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "DummyFactory.h"

#include "DummyBehaviour.h"
#include "DummyBehaviourB.h"
#include "DummyBehaviourD.h"
#include "DummyArbiter.h"

DummyFactory::DummyFactory()
{
    // the available behaviours
    add(new DummyBehaviour("Behaviour0"));
    add(new DummyBehaviour("Behaviour1"));
    add(new DummyBehaviour("Behaviour2"));
    add(new DummyBehaviour("Behaviour3"));
    add(new DummyBehaviourB("BehaviourB"));
    add(new DummyBehaviourD());

    // the available arbiters
    add(new DummyArbiter("Arbiter0"));
    add(new DummyArbiter("Arbiter1"));
}
