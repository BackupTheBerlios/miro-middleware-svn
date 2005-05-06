// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
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




