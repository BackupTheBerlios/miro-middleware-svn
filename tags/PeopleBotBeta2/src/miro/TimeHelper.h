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
#ifndef miroTimeHelper_hh
#define miroTimeHelper_hh

#include "miro/TimeC.h"

#include <iostream>

std::ostream& operator << (std::ostream& ostr, const ACE_Time_Value& rhs);
std::istream& operator >> (std::istream& istr, ACE_Time_Value& rhs);

namespace Miro
{
  inline
  void timeC2A(const TimeIDL& in, ACE_Time_Value& out) {
    out.sec(in.sec);
    out.usec(in.usec);
  }
  inline
  void timeA2C(const ACE_Time_Value& in, TimeIDL& out) {
    out.sec = in.sec();
    out.usec = in.usec();
  }
};

#endif




