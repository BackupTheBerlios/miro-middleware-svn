// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef XmlParse_h
#define XmlParse_h

#include "Exception.h"

#include <ace/TTY_IO.h>

#include <qdom.h>

#include <string>
#include <vector>
#include <set>

// forward declarations
class ACE_Time_Value;
class ACE_INET_Addr;


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
  class Angle;

  // XML parsing exceptions

  EXCEPTION_SUBTYPE(XmlException, Exception);
  EXCEPTION_SUBTYPE(InvalidXmlException, XmlException);
  EXCEPTION_SUBTYPE(UnnamedParameterException, XmlException);
  EXCEPTION_SUBTYPE(UnknownParameterException, XmlException);

  void operator <<= (bool& lhs, const QDomNode& node);
  void operator <<= (char& lhs, const QDomNode& node);
  void operator <<= (short& lhs, const QDomNode& node);
  void operator <<= (unsigned short& lhs, const QDomNode& node);
  void operator <<= (int& lhs, const QDomNode& node);
  void operator <<= (unsigned int& lhs, const QDomNode& node);
  void operator <<= (long& lhs, const QDomNode& node);
  void operator <<= (unsigned long& lhs, const QDomNode& node);
  void operator <<= (double& lhs, const QDomNode& node);
  void operator <<= (Angle& lhs, const QDomNode& node);
  void operator <<= (std::string& lhs, const QDomNode& node);
  void operator <<= (ACE_Time_Value& lhs, const QDomNode& node);
  void operator <<= (ACE_TTY_IO::Serial_Params& lhs, const QDomNode& node);
  void operator <<= (ACE_INET_Addr& lhs, const QDomNode& node);

  template<class T>
  void operator <<= (std::vector<T>& lhs, const QDomNode& node) {
    lhs.clear();
    QDomNode n = node.firstChild();
    while (!n.isNull()) {
      if (n.isElement()) {
	T t;
	t <<= n;
	lhs.push_back(t);
      }
      n = n.nextSibling();
    }
  }

  template<class T>
  void operator <<= (std::set<T>& lhs, const QDomNode& node) {
    lhs.clear();
    QDomNode n = node.firstChild();
    while (!n.isNull()) {
      if (n.isElement()) {
	T t;
	t <<= n;
	lhs.insert(t);
      }
      n = n.nextSibling();
    }
  }

  QDomElement operator >>= (const bool& lhs, QDomNode& node);
  QDomElement operator >>= (const char& lhs, QDomNode& node);
  QDomElement operator >>= (const short& lhs, QDomNode& node);
  QDomElement operator >>= (const unsigned short& lhs, QDomNode& node);
  QDomElement operator >>= (const int& lhs, QDomNode& node);
  QDomElement operator >>= (const unsigned int& lhs, QDomNode& node);
  QDomElement operator >>= (const long& lhs, QDomNode& node);
  QDomElement operator >>= (const unsigned long& lhs, QDomNode& node);
  QDomElement operator >>= (const double& lhs, QDomNode& node);
  QDomElement operator >>= (const Angle& lhs, QDomNode& node);
  QDomElement operator >>= (const std::string& lhs, QDomNode& node);
  QDomElement operator >>= (const ACE_Time_Value& lhs, QDomNode& node);
  QDomElement operator >>= (const ACE_TTY_IO::Serial_Params& lhs, QDomNode& node);
  QDomElement operator >>= (const ACE_INET_Addr& lhs, QDomNode& node);

  template<class T>
  QDomElement operator >>= (const std::vector<T>& lhs, QDomNode& node) {
    QDomDocument d = node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    typename std::vector<T>::const_iterator f, l = lhs.end();
    for (f = lhs.begin(); f != l; ++f) {
      (*f) >>= e;
    }
    node.appendChild(e);
    return e;
  }
  template<class T>
  QDomElement operator >>= (const std::set<T>& lhs, QDomNode& node) {
    QDomDocument d = node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    typename std::set<T>::const_iterator f, l = lhs.end();
    for (f = lhs.begin(); f != l; ++f) {
      (*f) >>= e;
    }
    node.appendChild(e);
    return e;
  }
};


#endif // xmlParse_h
