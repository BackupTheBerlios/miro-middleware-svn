// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroIO_h
#define miroIO_h

#include <iostream>

#define OSTR_OPERATOR_DECL(x) \
  class x; \
  std::ostream & \
  operator<<(std::ostream &ostr, const x &rhs)
#define ISTR_OPERATOR_DECL(x) \
  std::istream & \
  operator<<(std::istream &ostr, x &rhs)

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
  OSTR_OPERATOR_DECL(VelocityIDL);

  // interface specific types
  OSTR_OPERATOR_DECL(MotionStatusIDL);
  OSTR_OPERATOR_DECL(PanTiltPositionIDL);

  OSTR_OPERATOR_DECL(RangeGroupIDL);
  OSTR_OPERATOR_DECL(RangeScanIDL);
  OSTR_OPERATOR_DECL(RangeBunchEventIDL);
  OSTR_OPERATOR_DECL(RangeGroupEventIDL);
  OSTR_OPERATOR_DECL(RangeScanEventIDL);

  // b21 specific types
  //  OSTR_OPERATOR_DECL(LaserScanIDL);
};

#endif




