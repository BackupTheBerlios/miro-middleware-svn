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


#include "MotionParameters.h"
#include "Exception.h"

#include <qdom.h>

#include <cmath>

namespace Miro
{
  MotionParameters::MotionParameters() :
    minTranslation(-1000),
    maxTranslation( 1000),
    minRotation(-M_PI),
    maxRotation( M_PI)
  {}
   
  void
  MotionParameters::operator <<= (const QDomNode& _node)
  {
    QDomNode n = _node.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) { // the node was really an element.
	if (e.tagName()=="parameter") {
	  QDomAttr parameterName = e.attributeNode("name");
	  QString name;
	  QString value;

	  if (!parameterName.isNull()) {
	    name = parameterName.value();
	  }
	  else {
	    throw Exception("Parameter tag without name.");
	  }

	  QDomNode n2 = n.firstChild();
	  while (!n2.isNull()) {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    if(!t.isNull() ) {       // the node was really a text element.
	      value = t.data();
	      break;
	    }
	    n2 = n2.nextSibling();
	  }
	  if (n2.isNull())
	    throw Exception("Parameter " + std::string(name) + "without value.");
	  
	  if (name == "MinTranslation")
	    minTranslation = value.toInt();
	  else if (name == "MaxTranslation")
	    maxTranslation = value.toInt();
	  else if (name == "MinRotation")
	    minRotation = value.toDouble();
	  else if (name == "MaxRotation")
	    maxRotation = value.toDouble();
	}
      }
      n = n.nextSibling();
    }
  }

  void 
  MotionParameters::printToStream(std::ostream& _ostr) const
  {
    _ostr << "min translation: " << minTranslation << endl
	  << "max translation: " << maxTranslation << endl
	  << "min rotation: " << minRotation << endl
	  << "max rotation: " << maxRotation << endl;
  }

  DifferentialMotionParameters::DifferentialMotionParameters() :
    minLTranslation(-1000),
    maxLTranslation( 1000),
    minRTranslation(-1000),
    maxRTranslation( 1000),
    wheelBase(400)
  {}

  void
  DifferentialMotionParameters::operator <<= (const QDomNode& _node)
  {
    Super::operator<<=(_node);

    QDomNode n = _node.firstChild();
    while( !n.isNull() ) {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if( !e.isNull() ) {            // the node was really an element.
	if (e.tagName()=="parameter") {
	  QDomAttr parameterName = e.attributeNode("name");
	  QString name;
	  QString value;

	  if (!parameterName.isNull()) {
	    name = parameterName.value();
	  }
	  else {
	    throw Exception("Parameter tag without name.");
	  }

	  QDomNode n2 = n.firstChild();
	  while (!n2.isNull()) {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    if(!t.isNull() ) {        // the node was really a text element.
	      value = t.data();
	      break;
	    }
	    n2 = n2.nextSibling();
	  }
	  if (n2.isNull())
	    throw Exception("Parameter " + std::string(name) + "without value.");
	  
	  if (name == "MinLTranslation")
	    minLTranslation = value.toInt();
	  else if (name == "MaxLTranslation")
	    maxLTranslation = value.toInt();
	  else if (name == "MinRTranslation")
	    minRTranslation = value.toInt();
	  else if (name == "MaxRTranslation")
	    maxRTranslation = value.toInt();
	  else if (name == "WheelBase")
	    wheelBase = value.toInt();
	}
      }
      n = n.nextSibling();
    }
  }

  void 
  DifferentialMotionParameters::printToStream(std::ostream& _ostr) const
  {
    Super::printToStream(_ostr);

    _ostr << "min left translation: " << minLTranslation << endl
	  << "max left translation: " << maxLTranslation << endl
	  << "min right translation: " << minRTranslation << endl
	  << "max right translation: " << maxRTranslation << endl;
  }

};
