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
#ifndef miroIO_h
#define miroIO_h

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ace/TTY_IO.h>
#include <vector>
#include <set>
#include <iostream>

#define OSTR_OPERATOR_DECL(x) \
  class x; \
  std::ostream & \
  operator<<(std::ostream &ostr, x const &rhs)
#define ISTR_OPERATOR_DECL(x) \
  std::istream & \
  operator<<(std::istream &ostr, x &rhs)

std::ostream & 
operator<<(std::ostream &ostr, const ACE_TTY_IO::Serial_Params &rhs);
OSTR_OPERATOR_DECL(ACE_INET_Addr);
OSTR_OPERATOR_DECL(ACE_Sched_Params);

namespace std
{
  // set and vector ostream operators
  template<class T>
  std::ostream &
  operator<<(std::ostream &ostr, const std::vector<T> &rhs) {
    typename std::vector<T>::const_iterator first, last = rhs.end();
    for (first = rhs.begin(); first != last; ++first) {
      ostr << (*first) << std::endl;
    }
    return ostr;
  }
  template<class T>
  std::ostream &
  operator<<(std::ostream &ostr, const std::set<T> &rhs) {
    typename std::set<T>::const_iterator first, last = rhs.end();
    for (first = rhs.begin(); first != last; ++first) {
      ostr << (*first) << std::endl;
    }
    return ostr;
  }
}

namespace Miro 
{
  // exceptions
  OSTR_OPERATOR_DECL(EDevIO);
  OSTR_OPERATOR_DECL(ETimeOut);
  OSTR_OPERATOR_DECL(EOutOfBounds);

  // basic types
  OSTR_OPERATOR_DECL(TimeIDL);
  OSTR_OPERATOR_DECL(PositionIDL);
  OSTR_OPERATOR_DECL(WorldPointIDL);
  OSTR_OPERATOR_DECL(WorldPoint3DIDL);
  OSTR_OPERATOR_DECL(VelocityIDL);

  // interface specific types
  OSTR_OPERATOR_DECL(MotionStatusIDL);
  OSTR_OPERATOR_DECL(PanTiltPositionIDL);

  OSTR_OPERATOR_DECL(RangeGroupIDL);
  OSTR_OPERATOR_DECL(RangeScanIDL);
  OSTR_OPERATOR_DECL(RangeBunchEventIDL);
  OSTR_OPERATOR_DECL(RangeGroupEventIDL);
  OSTR_OPERATOR_DECL(RangeScanEventIDL);

  OSTR_OPERATOR_DECL(GpsPositionIDL);
  OSTR_OPERATOR_DECL(GpsDilutionIDL);
  OSTR_OPERATOR_DECL(GpsGlobalPositionEventIDL);
  OSTR_OPERATOR_DECL(GpsRelativePositionEventIDL);
  OSTR_OPERATOR_DECL(GpsPositionEventIDL);
  OSTR_OPERATOR_DECL(GpsDilutionEventIDL);
  OSTR_OPERATOR_DECL(GpsSentenceEventIDL);

  OSTR_OPERATOR_DECL(FieldStrengthIDL);
  OSTR_OPERATOR_DECL(InclinationIDL);

  OSTR_OPERATOR_DECL(CompassEventIDL);
  OSTR_OPERATOR_DECL(InclinometerEventIDL);
  OSTR_OPERATOR_DECL(MagnetometerEventIDL);
  OSTR_OPERATOR_DECL(ThermometerEventIDL);
#ifdef MIRO_HAS_PIONEER
  OSTR_OPERATOR_DECL(TCM2EventIDL);
#endif
}

#endif




