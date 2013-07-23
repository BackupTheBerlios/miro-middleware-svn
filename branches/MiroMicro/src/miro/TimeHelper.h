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
#ifndef miroTimeHelper_hh
#define miroTimeHelper_hh

#include "idl/TimeC.h"

#include <ace/Time_Value.h>
#include <ace/Version.h>

#include <iosfwd>
#include <string>


std::ostream& operator << (std::ostream& ostr, const ACE_Time_Value& rhs);
std::istream& operator >> (std::istream& istr, ACE_Time_Value& rhs);

namespace Miro
{
  inline
  void timeC2A(const TimeIDL& in, ACE_Time_Value& out)
  {
    out.sec(in.sec);
    out.usec(in.usec);
  }
  inline
  void timeA2C(const ACE_Time_Value& in, TimeIDL& out)
  {
    out.sec = in.sec();
    out.usec = in.usec();
  }

  std::string timeString();
  std::string timeStringSec();
  std::string timeStringMinute();

#if ACE_MAJOR_VERSION > 5 || (ACE_MAJOR_VERSION == 5  && ACE_MINOR_VERSION >= 4)
  std::string timeString(ACE_Time_Value const& _stamp);
  std::string timeStringSec(ACE_Time_Value const& _stamp);
  std::string timeStringMinute(ACE_Time_Value const& _stamp);
#endif
}

#endif




