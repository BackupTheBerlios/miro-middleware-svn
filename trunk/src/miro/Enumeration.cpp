// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

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
      string tmp = _values.substr(pos, _values.find(" ", pos)-pos);
      if ((tmp != " ") && (tmp != ""))
	values_.push_back(tmp);
      pos = _values.find(" ", pos)+1;
    }
    string tmp = _values.substr(pos, _values.size());
    if ((tmp != " ") && (tmp != ""))
      values_.push_back(tmp);

    // check if initial default value is in assortment list
    bool found = false;
    for (vector<string>::const_iterator i=values_.begin(); i!=values_.end(); ++i)
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
    for (vector<string>::const_iterator i=values_.begin(); i!=values_.end(); ++i)
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
    for (vector<string>::const_iterator i=values_.begin(); i!=values_.end(); ++i) {
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

