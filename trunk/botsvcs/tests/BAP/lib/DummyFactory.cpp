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
