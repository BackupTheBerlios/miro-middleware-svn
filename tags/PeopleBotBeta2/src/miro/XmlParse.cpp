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

#include "XmlParse.h"
#include "Angle.h"

#include <ace/INET_Addr.h>

#include <qstring.h>

#define XML_PARSE_QSTRING_IMPL(type, qstringmethod) \
  void operator <<= (type & _lhs, const QDomNode& _node) \
  { \
    bool valid; \
    QString value = getAttribute(_node, QString("value")); \
    _lhs = value.  qstringmethod  (&valid); \
    if (!valid) \
       throw Exception("Parse exception"); \
  } \
  QDomElement operator >>= (const type & _lhs, QDomNode& _node) \
  { \
    QString value; \
    QDomDocument document = _node.ownerDocument(); \
    QDomElement e = document. createElement("parameter"); \
    value.setNum(_lhs); \
    e.setAttribute("value", value); \
    _node.appendChild(e); \
    return e; \
  }

namespace
{
  const QString getAttribute(QDomNode _node, const QString& _attribute)
  {
    QString value;

    if( !_node.isNull() ) {
      QDomElement e = _node.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) {            // the node was really an element.
	QDomAttr attribute = e.attributeNode(_attribute);
	if (!attribute.isNull()) {
	  value = attribute.value();
	}
      }
    }
    return value;
  }
};

namespace Miro
{
  void operator <<= (bool& lhs, const QDomNode& node) 
  {
    QString value = getAttribute(node, QString("value"));
    if (value != "true" && value != "false")
      throw Exception("Parse exception");

    lhs = (value == "true");
  }

  QDomElement operator >>= (const bool& lhs, QDomNode& _node)
  {
    QDomDocument document = _node.ownerDocument();
    QDomElement e = document. createElement("parameter");
    e.setAttribute("value", QString((lhs)? "true" : "false"));
    _node.appendChild(e);
    return e;
  }

  void operator <<= (char& lhs, const QDomNode& node)
  {
    QString value = getAttribute(node, QString("value"));
    if (value.length() != 1)
      throw Exception("Parse exception");

    lhs = value[0].latin1();
  }

  QDomElement operator >>= (const char& lhs, QDomNode& _node)
  {
    QChar c(lhs);
    QDomDocument document = _node.ownerDocument();
    QDomElement e = document. createElement("parameter");
    e.setAttribute("value", QString(c));
    _node.appendChild(e);
    return e;
  }

  XML_PARSE_QSTRING_IMPL(short, toShort);
  XML_PARSE_QSTRING_IMPL(unsigned short, toUShort);
  XML_PARSE_QSTRING_IMPL(int, toInt);
  XML_PARSE_QSTRING_IMPL(unsigned int, toUInt);
  XML_PARSE_QSTRING_IMPL(long, toLong);
  XML_PARSE_QSTRING_IMPL(unsigned long, toULong);
  XML_PARSE_QSTRING_IMPL(float, toFloat);
  XML_PARSE_QSTRING_IMPL(double, toDouble);

  void operator <<= (Angle& lhs, const QDomNode& node)
  {
    bool valid;
    QString value = getAttribute(node, QString("value"));
    lhs = deg2Rad(value.toDouble(&valid));
    if (!valid)
      throw Exception("Parse exception");
  }

  QDomElement operator >>= (const Angle& lhs, QDomNode& _node)
  {
    QString value;
    QDomDocument document = _node.ownerDocument();
    QDomElement e = document. createElement("parameter");
    value.setNum(rad2Deg(lhs));
    e.setAttribute("value", value);
    _node.appendChild(e);
    return e;
  }

  void operator <<= (std::string& lhs, const QDomNode& node)
  {
    lhs = getAttribute(node, QString("value")).latin1();
  }

  QDomElement operator >>= (const std::string& lhs, QDomNode& _node)
  {
    QDomDocument document = _node.ownerDocument();
    QDomElement e = document. createElement("parameter");
    e.setAttribute("value", QString(lhs.c_str()));
    _node.appendChild(e);
    return e;
  }

  void operator <<= (ACE_Time_Value& lhs, const QDomNode& node)
  {
    bool valid;
    QString value = getAttribute(node, QString("value"));
    int dot = value.find('.');
    QString sec = value;
    QString usec = "0";
    if (dot > 0) {
      sec = value.left(dot);
      if (dot != ((int)value.length()) - 1)
	usec = value.mid(dot + 1);
    }

    lhs.sec(sec.toULong(&valid));
    if (!valid)
      throw Exception("Parse exception");
    lhs.usec(usec.toULong(&valid));
    if (!valid)
      throw Exception("Parse exception");
  }

  QDomElement operator >>= (const ACE_Time_Value& lhs, QDomNode& _node)
  {
    static const QString zero("000000");

    QString s;
    QString us;
    s.setNum(lhs.sec());
    us.setNum(lhs.usec());

    QDomDocument document = _node.ownerDocument();
    QDomElement e = document. createElement("parameter");
    e.setAttribute("value", s + "." + zero.left(6 - us.length()) + us);
    _node.appendChild(e);
    return e;
  }

  void
  operator<<= (ACE_TTY_IO::Serial_Params& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "Baudrate") {
              _lhs.baudrate <<= n;
            }
            else if (i == "Ctsenb") {
              _lhs.ctsenb <<= n;
            }
            else if (i == "Databits") {
              _lhs.databits <<= n;
            }
            else if (i == "Modem") {
              _lhs.modem <<= n;
            }
            else if (i == "Parityenb") {
              _lhs.parityenb <<= n;
            }
            else if (i == "Rcvenb") {
              _lhs.rcvenb <<= n;
            }
            else if (i == "Readtimeoutmsec") {
              _lhs.readtimeoutmsec <<= n;
            }
            else if (i == "Rtsenb") {
              _lhs.rtsenb <<= n;
            }
            else if (i == "Stopbits") {
              _lhs.stopbits <<= n;
            }
            else if (i == "Xinenb") {
              _lhs.xinenb <<= n;
            }
            else if (i == "Xofflim") {
              _lhs.xofflim <<= n;
            }
            else if (i == "Xonlim") {
              _lhs.xonlim <<= n;
            }
            else if (i == "Xoutenb") {
              _lhs.xoutenb <<= n;
            }
	    /*
            else if (i == "Paritymode") {
              _lhs.paritymode <<= n;
            }
	    */
          }
        }
        n = n.nextSibling();
      }
    }
  }
  QDomElement 
  operator>>= (const ACE_TTY_IO::Serial_Params& lhs, QDomNode& _node)
  {
    QDomDocument document = _node.ownerDocument();
    QDomElement e = document. createElement("parameter");

    QDomElement f;
    f = (lhs.baudrate >>= e);
    f.setAttribute("name", "Baudrate");
    f = (lhs.parityenb >>= e);
    f.setAttribute("name", "Parityenb");
    f = (lhs.databits >>= e);
    f.setAttribute("name", "Databits");
    f = (lhs.stopbits >>= e);
    f.setAttribute("name", "Stopbits");
    f = (lhs.readtimeoutmsec >>= e);
    f.setAttribute("name", "Readtimeoutmsec");
    f = (lhs.modem >>= e);
    f.setAttribute("name", "Modem");
    f = (lhs.rcvenb >>= e);
    f.setAttribute("name", "Rcvenb");
    f = (lhs.ctsenb >>= e);
    f.setAttribute("name", "Ctsenb");
    f = (lhs.rtsenb >>= e);
    f.setAttribute("name", "Rtsenb");
    f = (lhs.xinenb >>= e);
    f.setAttribute("name", "Xinenb");
    f = (lhs.xoutenb >>= e);
    f.setAttribute("name", "Xoutenb");

    _node.appendChild(e);
    return e;
  }

  void operator <<= (ACE_INET_Addr& lhs, const QDomNode& node)
  {
    lhs.set(getAttribute(node, QString("value")));
  }

  QDomElement operator >>= (const ACE_INET_Addr& lhs, QDomNode& _node)
  {
    QDomDocument document = _node.ownerDocument();
    QDomElement e = document. createElement("parameter");
    e.setAttribute("value", QString(lhs.get_host_addr()));
    _node.appendChild(e);
    return e;
  }
};


#ifdef ANALYZED_THAT
  void
  operator<<= (ACE_TTY_IO::Serial_Params& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "Baudrate") {
              _lhs.baudrate <<= n;
            }
            else if (i == "Ctsenb") {
              _lhs.ctsenb <<= n;
            }
            else if (i == "Databits") {
              _lhs.databits <<= n;
            }
            else if (i == "Modem") {
              _lhs.modem <<= n;
            }
            else if (i == "Parityenb") {
              _lhs.parityenb <<= n;
            }
            else if (i == "Rcvenb") {
              _lhs.rcvenb <<= n;
            }
            else if (i == "Readtimeoutmsec") {
              _lhs.readtimeoutmsec <<= n;
            }
            else if (i == "Rtsenb") {
              _lhs.rtsenb <<= n;
            }
            else if (i == "Stopbits") {
              _lhs.stopbits <<= n;
            }
            else if (i == "Xinenb") {
              _lhs.xinenb <<= n;
            }
            else if (i == "Xofflim") {
              _lhs.xofflim <<= n;
            }
            else if (i == "Xonlim") {
              _lhs.xonlim <<= n;
            }
            else if (i == "Xoutenb") {
              _lhs.xoutenb <<= n;
            }
            else if (i == "Paritymode") {
              _lhs.paritymode <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
  }
#endif
