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


#include "miro/TimeHelper.h"

#include <iostream>

std::ostream& operator << (std::ostream& ostr, const ACE_Time_Value& rhs) {
  ostr << rhs.sec() << ".";
  ostr.width(6);
  ostr.fill('0');
  ostr << rhs.usec();
  return ostr;
}

std::istream& operator >> (std::istream& istr, ACE_Time_Value& rhs) {
  char c;
  long t;

  istr >> t >> c;
  rhs.sec(t);

  if (c != '.')
    return istr;

  istr >> t;
  rhs.usec(t);
  return istr;
}



