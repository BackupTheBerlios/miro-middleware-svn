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
#include "Parameters.h"
#include "DummyBehaviourD.h"
#include "DummyArbiterMessage.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

unsigned long DummyBehaviourD::counter_ = 0;

const std::string DummyBehaviourD::name_ = "DummyBehaviourD";

DummyBehaviourD::DummyBehaviourD(const std::string& _transition) :
  Super(*ACE_Reactor::instance()),
  transition_(_transition)
{
  std::cout << "Constructing " << name_ << "." << endl;
}

DummyBehaviourD::~DummyBehaviourD()
{
  std::cout << "Destructing " << name_ << "." << endl;
}

void
DummyBehaviourD::action()
{
  std::cout << name_ << " action: " << counter_ << endl;

  DummyArbiterMessage message;
  message.id = this;
  message.active = false;
  message.message = "Action of " + name_;
  arbitrate(message);

  Miro::BAP::BehaviourParameters * const p0 = 
    getSuccessorBehaviourParameters(transition_, "Behaviour0");
  DummyBehaviourParameters * const p =
    dynamic_cast<DummyBehaviourParameters * const>(p0);

  p->dummy *= 3;

  setSuccessorBehaviourParameters(transition_, "Behaviour0", p);
}

const std::string&
DummyBehaviourD::getBehaviourName() const
{
  return name_;
}
