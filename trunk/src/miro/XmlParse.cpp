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

#include "XmlParse.h"
#include "Exception.h"
//#include "ScanDescriptionC.h"

#include <ace/Time_Value.h>

#include <qdom.h>
#include <qstring.h>

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

  void operator <<= (char& lhs, const QDomNode& node)
  {
    QString value = getAttribute(node, QString("value"));
    if (value.length() != 1)
      throw Exception("Parse exception");

    lhs = value[0].latin1();
  }

  void operator <<= (short& lhs, const QDomNode& node)
  {
    QString value = getAttribute(node, QString("value"));
    bool valid;

    value.toShort(&valid);
    if (!valid)
      throw Exception("Parse exception");

    lhs = value.toShort();
  }

  void operator <<= (int& lhs, const QDomNode& node)
  {
    bool valid;

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomText t = n1.toText(); // try to convert the node to a text
	if(!t.isNull() ) {        // the node was really a text element.
	  t.data().toDouble(&valid);
	  if (!valid)
	    throw Exception("Parse exception");
	  
	  lhs = t.data().toInt();
	  break;
	}
	n1 = n1.nextSibling();
      }
    }
    else
      throw Exception("Parse exception");
  }

  void operator <<= (unsigned long& lhs, const QDomNode& node)
  {
    bool valid;

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomText t = n1.toText(); // try to convert the node to a text
	if(!t.isNull() ) {        // the node was really a text element.
	  t.data().toDouble(&valid);
	  if (!valid)
	    throw Exception("Parse exception");
	  
	  lhs = t.data().toInt();
	  break;
	}
	n1 = n1.nextSibling();
      }
    }
    else
      throw Exception("Parse exception");

  }

  void operator <<= (double& lhs, const QDomNode& node)
  {
    bool valid;

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomText t = n1.toText(); // try to convert the node to a text
	if(!t.isNull() ) {        // the node was really a text element.
	  t.data().toDouble(&valid);
	  if (!valid)
	    throw Exception("Parse exception");
	  
	  lhs = t.data().toDouble();
	  break;
	}
	n1 = n1.nextSibling();
      }
    }
    else
      throw Exception("Parse exception");
  }

  void operator <<= (Angle& lhs, const QDomNode& node)
  {
    bool valid;

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomText t = n1.toText(); // try to convert the node to a text
	if(!t.isNull() ) {        // the node was really a text element.
	  t.data().toDouble(&valid);
	  if (!valid)
	    throw Exception("Parse exception");
	  
	  lhs = deg2Rad(t.data().toDouble());
	  break;
	}
	n1 = n1.nextSibling();
      }
    }
    else
      throw Exception("Parse exception");
  }

  void operator <<= (std::string& lhs, const QDomNode& node)
  {
    lhs = getAttribute(node, QString("value"));
  }

  void operator <<= (ACE_Time_Value& lhs, const QDomNode& node)
  {
    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomNode n2 = n1.firstChild();
	if (!n2.isNull()) {
	  QDomText t = n2.toText(); // try to convert the node to a text
	  bool valid;
	  t.data().toInt(&valid);
	  if (!valid)
	    throw Exception("Parse exception");
	  if(!t.isNull() ) {       // the node was really a text element.
	    if (n1.nodeName() == "sec") {
	      lhs.sec(t.data().toInt());
	    }
	    else if (n1.nodeName() == "usec") {
	      lhs.usec(t.data().toInt());
	    }
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  void operator <<= (ACE_TTY_IO::Serial_Params& lhs, const QDomNode& node)
  {
    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomNode n2 = n1.firstChild();
	if (!n2.isNull()) {
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) {       // the node was really a text element.
	    if (n1.nodeName() == "baudrate") {
	      lhs.baudrate = t.data().toInt();
	    }
	    else if (n1.nodeName() == "parityenb") {
	      lhs.parityenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "databits") {
	      lhs.databits = t.data().toInt();
	    }
	    else if (n1.nodeName() == "stopbits") {
	      lhs.stopbits = t.data().toInt();
	    }
	    else if (n1.nodeName() == "readtimeoutmsec") {
	      lhs.readtimeoutmsec = t.data().toInt();
	    }
	    else if (n1.nodeName() == "modem") {
	      lhs.modem = t.data().toInt();
	    }
	    else if (n1.nodeName() == "rcvenb") {
	      lhs.rcvenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "ctsenb") {
	      lhs.ctsenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "rtsenb") {
	      lhs.rtsenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "xinenb") {
	      lhs.xinenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "xoutenb") {
	      lhs.xoutenb = t.data().toInt();
	    }
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  //  void operator <<= (ScanDescriptionIDL& lhs, const QDomNode& node)
  //  {
  //  }

};
