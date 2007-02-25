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
#ifndef Transition_hh
#define Transition_hh

#include <qstring.h>

#include <vector>

// forward declarations
class PatternXML;

//-----------------------------------------------------------------

class Transition
{
private:
  QString  message;         // message name
  PatternXML *  target;          // target pattern name

public:
  Transition() { }
  Transition(const QString& msg, PatternXML * tar);

  PatternXML * getTarget() const;
  const QString& getMessage() const;

  bool operator < (Transition const& _rhs) const;
};

typedef std::vector<Transition> TransitionVector;

inline
Transition::Transition(const QString& msg, PatternXML * tar) :
  message(msg),
  target(tar) 
{}

inline
PatternXML * 
Transition::getTarget() const
{
  return target; 
}

inline
const QString&
Transition::getMessage() const 
{
  return message; 
}

inline
bool
Transition::operator < (Transition const& _rhs) const {
  return message < _rhs.message;
}
#endif
