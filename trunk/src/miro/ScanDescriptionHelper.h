// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef ScanDescriptionHelper_h
#define ScanDescriptionHelper_h

#include <iostream>
// forward declarations
class QDomNode;

namespace Miro
{
  class SensorDescriptionIDL;
  class SensorPositionIDL;
  class SensorGroupIDL;
  class ScanDescriptionIDL;

  void operator <<= (SensorDescriptionIDL& position, const QDomNode& node);
  void operator <<= (SensorPositionIDL& position, const QDomNode& node);
  void operator <<= (SensorGroupIDL& position, const QDomNode& node);
  void operator <<= (ScanDescriptionIDL& description, const QDomNode& node);

  std::ostream&
  operator << (std::ostream& ostr, const SensorDescriptionIDL& description);
  std::ostream&
  operator << (std::ostream& ostr, const SensorPositionIDL& position);
  std::ostream&
  operator << (std::ostream& ostr, const SensorGroupIDL& group);
  std::ostream&
  operator << (std::ostream& ostr, const ScanDescriptionIDL& description);
};
#endif // scanDescriptionHelper_hh
