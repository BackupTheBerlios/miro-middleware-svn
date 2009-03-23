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
