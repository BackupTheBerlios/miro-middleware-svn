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
#ifndef DummyArbiter_h
#define DummyArbiter_h

#include "botsvcs/BAP/PriorityArbiter.h"
#include "DummyArbiterMessage.h"

class DummyArbiter : public Miro::BAP::PriorityArbiter
{
  //! Super class type.
  typedef Miro::BAP::PriorityArbiter Super;
  
public:
  DummyArbiter(const std::string& _name);
  
  // factory method for ArbiterParameters
  DummyArbiterMessage * getMessageInstance() const;
  
  virtual const std::string& getName() const;

protected:
  virtual void setActuators(const Miro::BAP::ArbiterMessage& _message);
  virtual void limpActuators();
  
  std::string message_;

  //! Arbiter name.
  const std::string name_;
};
#endif
