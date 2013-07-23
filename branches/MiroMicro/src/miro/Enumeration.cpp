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
//
// Authors:
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#include "Enumeration.h"
#include "miro/Exception.h"

#include <iostream>

using std::string;
using std::vector;


namespace Miro
{
  Enumeration::Enumeration(string _enum, string _values) :
      enum_(_enum)
  {
    // tokenize input values
    int pos = 0;
    while (_values.find(" ", pos) < _values.size()) {
      string tmp = _values.substr(pos, _values.find(" ", pos) - pos);
      if ((tmp != " ") && (tmp != ""))
        values_.push_back(tmp);
      pos = _values.find(" ", pos) + 1;
    }
    string tmp = _values.substr(pos, _values.size());
    if ((tmp != " ") && (tmp != ""))
      values_.push_back(tmp);

    // check if initial default value is in assortment list
    bool found = false;
    for (vector<string>::const_iterator i = values_.begin(); i != values_.end(); ++i)
      if (*i == enum_) {
        found = true;
        break;
      }
    if (!found)
      throw Miro::Exception("Enumeration: tried to set inital value not in list of available values");
  }


  Enumeration::Enumeration(string _enum, vector<string> _values) :
      enum_(_enum),
      values_(_values)
  {
    // check if initial default value is in assortment list
    bool found = false;
    for (vector<string>::const_iterator i = values_.begin(); i != values_.end(); ++i)
      if (*i == enum_) {
        found = true;
        break;
      }
    if (!found)
      throw Miro::Exception("Enumeration: tried to set inital value not in list of available values");
  }


  void
  Enumeration::value(string _value)
  {
    bool found = false;
    for (vector<string>::const_iterator i = values_.begin(); i != values_.end(); ++i) {
      if (_value == *i) {
        enum_ = *i;
        found = true;
        break;
      }
    }
    if (!found)
      throw Miro::Exception("Enumeration: tried to set value not in list of available values");
  }


  const
  string&
  Enumeration::value() const
  {
    return enum_;
  }


  const
  vector<string>&
  Enumeration::assortment() const
  {
    return values_;
  }


  std::ostream&
  operator<<(std::ostream& ostr, Enumeration _enum)
  {
    return ostr << _enum.enum_;
  }

  std::istream&
  operator>>(std::istream& istr, Enumeration& _enum)
  {
    std::string tmp;
    istr >> tmp;
    _enum.value(tmp); // this way, error handling is done by value()
    return istr;
  }
}

