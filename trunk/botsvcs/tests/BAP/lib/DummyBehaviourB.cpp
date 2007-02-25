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
#include "DummyBehaviourB.h"
#include "DummyArbiterMessage.h"

unsigned long DummyBehaviourB::counter_ = 0;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

DummyBehaviourB::DummyBehaviourB(const std::string& _name,
				 const std::string& _transition) :
  Super(*ACE_Reactor::instance()),
  transition_(_transition),
  name_(_name)
{
  std::cout << "Constructing " << name_ << "." << endl;
}

DummyBehaviourB::~DummyBehaviourB()
{
  std::cout << "Destructing " << name_ << "." << endl;
}

void
DummyBehaviourB::action()
{
  std::cout << name_ << " action: " << counter_ << endl;

  DummyArbiterMessage message;
  message.id = this;
  message.active = false;
  message.message = "Action of " + name_;
  arbitrate(message);

  if (!(++counter_ % 2))
    sendMessage(transition_);
}

const std::string&
DummyBehaviourB::getBehaviourName() const
{
  return name_;
}
