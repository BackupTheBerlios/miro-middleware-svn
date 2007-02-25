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
#ifndef DummyBehaviour_h
#define DummyBehaviour_h

#include "botsvcs/tests/BAP/lib/Parameters.h"
#include "botsvcs/BAP/TimedBehaviour.h"

class DummyBehaviour : public Miro::BAP::TimedBehaviour
{
  typedef Miro::BAP::TimedBehaviour Super;

public:
  DummyBehaviour(const std::string& _name);
  virtual ~DummyBehaviour();

  BEHAVIOUR_PARAMETERS_FACTORY(DummyBehaviourParameters);
  virtual void init(Miro::BAP::BehaviourParameters const * _params);
  virtual std::string actionTake2(Miro::BAP::BehaviourParameters const * _params,
				  Miro::BAP::ArbiterMessage * _message);
  virtual const std::string& getBehaviourName() const;

protected:
  bool active_;
  int sendTransition_;
  
private:
  const std::string name_;
};

#endif
