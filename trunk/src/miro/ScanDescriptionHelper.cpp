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

#include "ScanDescriptionHelper.h"
#include "ScanDescriptionC.h"

#include "Angle.h"

#include <qdom.h>
#include <iostream>

namespace Miro
{
  void
  operator <<= (SensorDescriptionIDL& sensor, const QDomNode& node)
  {
    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomNode n2 = n1.firstChild();
	if (!n2.isNull()) {
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) {       // the node was really a text element.
	    if (n1.nodeName() == "minrange") {
	      sensor.minRange = t.data().toInt();
	    }
	    else if (n1.nodeName() == "maxrange") {
	      sensor.maxRange = t.data().toInt();
	    }
	    else if (n1.nodeName() == "focus") {
	      sensor.focus = deg2Rad(t.data().toDouble());
	    }
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  void
  operator <<= (SensorPositionIDL& position, const QDomNode& node)
  {
    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomNode n2 = n1.firstChild();
	if (!n2.isNull()) {
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) {       // the node was really a text element.
	    if (n1.nodeName() == "height") {
	      position.height = t.data().toInt();
	    }
	    else if (n1.nodeName() == "distance") {
	      position.distance = t.data().toInt();
	    }
	    else if (n1.nodeName() == "alpha") {
	      position.alpha = deg2Rad(t.data().toDouble());
	    }
	    else if (n1.nodeName() == "beta") {
	      position.beta = deg2Rad(t.data().toDouble());
	    }
	    else if (n1.nodeName() == "gamma") {
	      position.gamma = deg2Rad(t.data().toDouble());
	    }
	    else if (n1.nodeName() == "masked") {
	      position.masked = (t.data() == "true");
	    }
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  void
  operator <<= (Miro::SensorGroupIDL& group, const QDomNode& node)
  {
    if (!node.isNull()) {

      // set sensor defaults
      Miro::SensorPositionIDL sensor;
      sensor.masked = false;
      QDomNamedNodeMap map = node.attributes();
      QDomNode n;
      n = map.namedItem("height");
      if (!n.isNull()) {
	QDomAttr attr = n.toAttr();
	if (!attr.isNull()) 
	  sensor.height = attr.value().toInt();
      }
      n = map.namedItem("distance");
      if (!n.isNull()) {
	QDomAttr attr = n.toAttr();
	if (!attr.isNull()) 
	  sensor.distance =attr.value().toInt();
      }
      n = map.namedItem("alpha");
      if (!n.isNull()) {
	QDomAttr attr = n.toAttr();
	if (!attr.isNull()) 
	  sensor.alpha = deg2Rad(attr.value().toDouble());
      }
      n = map.namedItem("beta");
      if (!n.isNull()) {
	QDomAttr attr = n.toAttr();
	if (!attr.isNull()) 
	  sensor.beta = deg2Rad(attr.value().toDouble());
      }
      n = map.namedItem("gamma");
      if (!n.isNull()) {
	QDomAttr attr = n.toAttr();
	if (!attr.isNull()) 
	  sensor.gamma = deg2Rad(attr.value().toDouble());
      }
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	if (n1.nodeName() == "description") {
	  group.description <<= n1;
	}
	else if (n1.nodeName() == "sensor") {
	  group.sensor.length(group.sensor.length() + 1);
	  group.sensor[group.sensor.length() - 1] = sensor;
	  group.sensor[group.sensor.length() - 1] <<= n1;
	}
	n1 = n1.nextSibling();
      }
    }
  }

  void
  operator <<= (ScanDescriptionIDL& description, const QDomNode& node)
  {
    description.group.length(0);

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	if (n1.nodeName() == "sensorgroup") {
	  description.group.length(description.group.length() + 1);
	  description.group[description.group.length() - 1].sensor.length(0);
	  description.group[description.group.length() - 1] <<= n1;
	} 
	else if (n1.nodeName() == "scantype") {
	  QDomNode n2 = n1.firstChild();
	  if (!n2.isNull()) {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    description.scanType = t.data().toInt();
	  }
	}
	else if (n1.nodeName() == "eventname") {
	  QDomNode n2 = n1.firstChild();
	  if (!n2.isNull()) {
	    QDomText t = n2.toText(); // try to convert the node to a text
	    description.eventName = CORBA::string_dup(t.data());
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  std::ostream&
  operator << (std::ostream& ostr, const SensorDescriptionIDL& description) 
  {
    ostr << " minrange=" << description.minRange
	 << " maxrange=" << description.maxRange
	 << " focus=" << description.focus;
    return ostr;
  }

  std::ostream&
  operator << (std::ostream& ostr, const SensorPositionIDL& position) 
  {
    ostr << " height=" << position.height
	 << " distance=" << position.distance
	 << " alpha=" << rad2Deg(position.alpha)
	 << " beta=" << rad2Deg(position.beta)
	 << " gamma=" << rad2Deg(position.gamma)
	 << " masked=" << (position.masked? "true" : "false");
    return ostr;
  }

  std::ostream&
  operator << (std::ostream& ostr, const SensorGroupIDL& group) 
  {
    ostr << "description: " << group.description;
    for (unsigned int i = 0; i < group.sensor.length(); ++i) {
      ostr << endl 
	   << "sensor " << i << ": "
	   << group.sensor[i];
    }
    
    return ostr;
  }

  std::ostream&
  operator << (std::ostream& ostr, const ScanDescriptionIDL& description) 
  {
    ostr << "  scan description:" << endl
	 << "    scan type=" << description.scanType
	 << "    event name=" << description.eventName;
    for (unsigned int i = 0; i < description.group.length(); ++i) {
      ostr << endl 
	   << "    group " << i << ":" << endl
	   << description.group[i];
    }
    
    return ostr;
  }
};
