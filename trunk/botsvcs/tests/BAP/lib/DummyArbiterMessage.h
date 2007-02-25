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
#ifndef DummyArbiterMessage_h
#define DummyArbiterMessage_h

#include "botsvcs/BAP/ArbiterMessage.h"

#include <string>

//! Arbitration message for the wheel power arbiter.
class DummyArbiterMessage : public Miro::BAP::ArbiterMessage
{
public:
  //! The requested left wheel power.
  std::string message;

  //! Default constructor. 
  DummyArbiterMessage();
  virtual void assign(const ArbiterMessage& _lhs);

protected:
  //! Dump message to the specified output stream.
  virtual void printToStream(std::ostream& ostr) const;
private:
  //! Type of this class.
  typedef DummyArbiterMessage Self;
  //! Super class type.
  typedef ArbiterMessage Super;
};
#endif
