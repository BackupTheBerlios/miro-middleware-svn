// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef XmlParse_h
#define XmlParse_h

#include "Angle.h"

#include <string>
#include <ace/TTY_IO.h>

// forward declarations
class ACE_Time_Value;
class QDomNode;

namespace Miro
{
  // forward declarations
  //  struct ScanDescriptionIDL;

  void operator <<= (bool& lhs, const QDomNode& node);
  void operator <<= (char& lhs, const QDomNode& node);
  void operator <<= (short& lhs, const QDomNode& node);
  void operator <<= (int& lhs, const QDomNode& node);
  void operator <<= (unsigned long& lhs, const QDomNode& node);
  void operator <<= (double& lhs, const QDomNode& node);
  void operator <<= (Angle& lhs, const QDomNode& node);
  void operator <<= (std::string& lhs, const QDomNode& node);
  void operator <<= (ACE_Time_Value& lhs, const QDomNode& node);
  void operator <<= (ACE_TTY_IO::Serial_Params& lhs, const QDomNode& node);
  //  void operator <<= (ScanDescriptionIDL& lhs, const QDomNode& node);
};
#endif // xmlParse_hh
