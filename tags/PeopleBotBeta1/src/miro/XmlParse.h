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
#include "Exception.h"

#include <string>
#include <ace/TTY_IO.h>

// forward declarations
class ACE_Time_Value;
class QDomNode;

#define EXCEPTION_SUBTYPE(type, parent) \
  class type : public parent \
  { \
    typedef parent Super; \
  public: \
    /*! Default constructor. */ \
    type() throw() : Super() {} \
    /*! Initializing constructor */ \
    type(const std::string& _what) throw() : Super(_what) {} \
  }

namespace Miro
{
  // forward declarations
  //  struct ScanDescriptionIDL;

  // XML parsing exceptions

  EXCEPTION_SUBTYPE(XmlException, Exception);
  EXCEPTION_SUBTYPE(InvalidXmlException, XmlException);
  EXCEPTION_SUBTYPE(UnnamedParameterException, XmlException);
  EXCEPTION_SUBTYPE(UnknownParameterException, XmlException);

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
