// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef ScanDescriptionHelper_h
#define ScanDescriptionHelper_h

#include <qdom.h>

#include <iosfwd>

namespace Miro
{
  // forward declarations
  class SensorDescriptionIDL;
  class SensorPositionIDL;
  class SensorGroupIDL;
  class ScanDescriptionIDL;

  void operator <<= (SensorDescriptionIDL& position, const QDomNode& node);
  void operator <<= (SensorPositionIDL& position, const QDomNode& node);
  void operator <<= (SensorGroupIDL& position, const QDomNode& node);
  void operator <<= (ScanDescriptionIDL& description, const QDomNode& node);
  QDomElement operator >>= (const SensorDescriptionIDL& position, QDomNode& node);
  QDomElement operator >>= (const SensorPositionIDL& position, QDomNode& node);
  QDomElement operator >>= (const SensorGroupIDL& position, QDomNode& node);
  QDomElement operator >>= (const ScanDescriptionIDL& description, QDomNode& node);

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
