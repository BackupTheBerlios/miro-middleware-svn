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

#include "EnumerationMultiple.h"
#include "miro/Exception.h"

#include <iostream>

using std::string;
using std::vector;


namespace Miro
{
  EnumerationMultiple::EnumerationMultiple(string _enum, string _values)
  {
    enum_ = tokenizer(_enum);
    values_ = tokenizer(_values);
    checkAvailability();
  }


  EnumerationMultiple::EnumerationMultiple(vector<string> _enum, vector<string> _values) :
    enum_(_enum),
    values_(_values)
  {
    checkAvailability();
  }

    
  void
  EnumerationMultiple::value(string _value)
  {
    enum_ = tokenizer(_value);
    checkAvailability();
  }


  void
  EnumerationMultiple::value(vector<string> _value)
  {
    enum_ = _value;
    checkAvailability();
  }


  const 
  vector<string>&
  EnumerationMultiple::value() const
  {
    return enum_;
  }


  const 
  vector<string>&
  EnumerationMultiple::assortment() const
  {
    return values_;
  }


  std::vector<std::string>
  EnumerationMultiple::tokenizer(std::string _values)
  {
    int pos = 0;
    std::vector<std::string> values;
    while (_values.find(" ", pos) < _values.size()) {
      string tmp = _values.substr(pos, _values.find(" ", pos)-pos);
      if ((tmp != " ") && (tmp != ""))
	values.push_back(tmp);
      pos = _values.find(" ", pos)+1;
    }
    string tmp = _values.substr(pos, _values.size());
    if ((tmp != " ") && (tmp != ""))
      values.push_back(tmp);

    return values;
  }


  void
  EnumerationMultiple::checkAvailability()
  {
    // check if values are in assortment list
    for (vector<string>::const_iterator j=enum_.begin(); j!=enum_.end(); ++j) {
      bool found = false;
      for (vector<string>::const_iterator i=values_.begin(); i!=values_.end(); ++i)
	if (*i == *j) {
	  found = true;
	  break;
	}
      if (!found)
	throw Miro::Exception("Enumeration: tried to set value not in list of available values");
    }
  }


  std::ostream& 
  operator<<(std::ostream& ostr, EnumerationMultiple _enum)
  {
    for (vector<string>::const_iterator j=_enum.enum_.begin(); j!=_enum.enum_.end(); ++j)
      ostr << *j;
    return ostr;
  }


  std::istream& 
  operator>>(std::istream& istr, EnumerationMultiple& _enum)
  {
    std::string tmp;
    istr >> tmp;
    _enum.value(tmp); // this way, error handling is done by value()
    return istr;
  }
}

