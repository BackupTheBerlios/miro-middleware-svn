// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroTimeHelper_hh
#define miroTimeHelper_hh

#include "idl/TimeC.h"

#include <ace/Time_Value.h>

#include <iostream>
#include <string>


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

  std::string timeString(ACE_Time_Value const& _stamp = ACE_OS::gettimeofday());
  std::string timeStringSec(ACE_Time_Value const& _stamp = ACE_OS::gettimeofday());
  std::string timeStringMinute(ACE_Time_Value const& _stamp = ACE_OS::gettimeofday());
};

#endif




