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

#include "ScanDescriptionHelper.h"
#include "ScanDescriptionC.h"
#include "XmlParse.h"

#include "Angle.h"

namespace Miro
{
  void
  operator<<= (SensorDescriptionIDL& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "Focus") {
	      _lhs.focus <<= n;
              _lhs.focus = Miro::deg2Rad(_lhs.focus);
            }
            else if (i == "MaxRange") {
              _lhs.maxRange <<= n;
            }
            else if (i == "MinRange") {
              _lhs.minRange <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
  }

  QDomElement
  operator>>= (const SensorDescriptionIDL& _lhs, QDomNode& _node)
  {
    QDomDocument d = _node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    _node.appendChild(e);

    QDomElement g;

    g = (Miro::rad2Deg(_lhs.focus) >>= e);
    g.setAttribute("name", "Focus");
    g = (_lhs.maxRange >>= e);
    g.setAttribute("name", "MaxRange");
    g = (_lhs.minRange >>= e);
    g.setAttribute("name", "MinRange");

    return e;
  }

  void
  operator<<= (SensorPositionIDL& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
	    double angle;
	    bool truth;
            if (i == "Alpha") {
	      angle <<= n;
              _lhs.alpha = Miro::deg2Rad(angle);
            }
            else if (i == "Beta") {
	      angle <<= n;
              _lhs.beta = Miro::deg2Rad(angle);
            }
            else if (i == "Gamma") {
	      angle <<= n;
              _lhs.gamma = Miro::deg2Rad(angle);
            }
            else if (i == "Masked") {
	      truth <<= n;
              _lhs.masked = truth;
            }
            else if (i == "Distance") {
	      _lhs.distance <<= n;
            }
            else if (i == "Height") {
	      _lhs.height <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
  }

  QDomElement
  operator>>= (const SensorPositionIDL& _lhs, QDomNode& _node)
  {
    QDomDocument d = _node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    _node.appendChild(e);

    QDomElement g;

    g = (Miro::rad2Deg(_lhs.alpha) >>= e);
    g.setAttribute("name", "Alpha");
    g = (Miro::rad2Deg(_lhs.beta) >>= e);
    g.setAttribute("name", "Beta");
    g = (Miro::rad2Deg(_lhs.gamma) >>= e);
    g.setAttribute("name", "Gamma");
    g = ((bool)_lhs.masked >>= e);
    g.setAttribute("name", "Masked");
    g = (_lhs.distance >>= e);
    g.setAttribute("name", "Distance");
    g = (_lhs.height >>= e);
    g.setAttribute("name", "Height");

    return e;
  }

  void
  operator<<= (SensorGroupIDL& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "Description") {
              _lhs.description <<= n;
            }
            else if (i == "Sensor") {
	      _lhs.sensor.length(_lhs.sensor.length() + 1);
	      _lhs.sensor[_lhs.sensor.length() - 1] <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
  }

  QDomElement
  operator>>= (const SensorGroupIDL& _lhs, QDomNode& _node)
  {
    QDomDocument d = _node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    _node.appendChild(e);

    QDomElement g;

    g = (_lhs.description >>= e);
    g.setAttribute("name", "Description");


    g = d.createElement("parameter");
    e.appendChild(g);
    for (unsigned long i = 0; i < _lhs.sensor.length(); ++i) {
      _lhs.sensor[i] >>= g;
    }
    g.setAttribute("name", "Sensor");

    return e;
  }

  void
  operator<<= (ScanDescriptionIDL& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "EventName") {
	      std::string s;
	      s <<= n;
              _lhs.eventName = CORBA::string_dup(s.c_str());
            }
            else if (i == "Group") {
	      unsigned long len = _lhs.group.length() + 1;
	      _lhs.group.length(len);
	      _lhs.group[len - 1].sensor.length(0);
	      _lhs.group[len - 1] <<= n;
            }
            else if (i == "ScanType") {
              _lhs.scanType <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
  }

  QDomElement
  operator>>= (const ScanDescriptionIDL& _lhs, QDomNode& _node)
  {
    QDomDocument d = _node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    _node.appendChild(e);

    QDomElement g;

    g = ((long)_lhs.scanType >>= e);
    g.setAttribute("name", "ScanType");
    g = (std::string(_lhs.eventName) >>= e);
    g.setAttribute("name", "EventName");

    g = d.createElement("parameter");
    e.appendChild(g);
    for (unsigned long i = 0; i < _lhs.group.length(); ++i) {
      _lhs.group[i] >>= g;
    }
    g.setAttribute("name", "Group");

    return e;
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


#ifdef ANALYZED_THAT
  void
  operator<<= (SensorDescriptionIDL& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "Focus") {
              _lhs.focus <<= n;
              _lhs.focus = Miro::deg2Rad(_lhs.focus);
            }
            else if (i == "MaxRange") {
              _lhs.maxRange <<= n;
            }
            else if (i == "MinRange") {
              _lhs.minRange <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
  }
  QDomElement
  operator>>= (const SensorDescriptionIDL& _lhs, QDomNode& _node)
  {
    QDomDocument d = _node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    _node.appendChild(e);

    QDomElement g;

    g = (_lhs.focus >>= e);
    g.setAttribute("name", "Focus");
    g = (_lhs.maxRange >>= e);
    g.setAttribute("name", "MaxRange");
    g = (_lhs.minRange >>= e);
    g.setAttribute("name", "MinRange");

    return e;
  }

  void
  operator<<= (SensorPositionIDL& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "Alpha") {
              _lhs.alpha <<= n;
              _lhs.alpha = Miro::deg2Rad(_lhs.alpha);
            }
            else if (i == "Beta") {
              _lhs.beta <<= n;
              _lhs.beta = Miro::deg2Rad(_lhs.beta);
            }
            else if (i == "Gamma") {
              _lhs.gamma <<= n;
              _lhs.gamma = Miro::deg2Rad(_lhs.gamma);
            }
            else if (i == "Masked") {
              _lhs.masked <<= n;
            }
            else if (i == "Distance") {
              _lhs.distance <<= n;
            }
            else if (i == "Height") {
              _lhs.height <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
  }
  QDomElement
  operator>>= (const SensorPositionIDL& _lhs, QDomNode& _node)
  {
    QDomDocument d = _node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    _node.appendChild(e);

    QDomElement g;

    g = (_lhs.alpha >>= e);
    g.setAttribute("name", "Alpha");
    g = (_lhs.beta >>= e);
    g.setAttribute("name", "Beta");
    g = (_lhs.gamma >>= e);
    g.setAttribute("name", "Gamma");
    g = (_lhs.masked >>= e);
    g.setAttribute("name", "Masked");
    g = (_lhs.distance >>= e);
    g.setAttribute("name", "Distance");
    g = (_lhs.height >>= e);
    g.setAttribute("name", "Height");

    return e;
  }

  void
  operator<<= (SensorGroupIDL& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "Description") {
              _lhs.description <<= n;
            }
            else if (i == "Sensor") {
              _lhs.sensor <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
  }
  QDomElement
  operator>>= (const SensorGroupIDL& _lhs, QDomNode& _node)
  {
    QDomDocument d = _node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    _node.appendChild(e);

    QDomElement g;

    g = (_lhs.description >>= e);
    g.setAttribute("name", "Description");
    g = (_lhs.sensor >>= e);
    g.setAttribute("name", "Sensor");

    return e;
  }

  void
  operator<<= (ScanDescriptionIDL& _lhs, const QDomNode& _node)
  {
    if (!_node.isNull()) {
      QDomNode n = _node.firstChild();
      while(!n.isNull() ) {
        QDomElement e = n.toElement();
        if( !e.isNull() ) {
          QDomAttr a = e.attributeNode("name");
          if (!a.isNull()) {
            QString i = a.value();
            if (i == "EventName") {
              _lhs.eventName <<= n;
            }
            else if (i == "Group") {
              _lhs.group <<= n;
            }
            else if (i == "ScanType") {
              _lhs.scanType <<= n;
            }
          }
        }
        n = n.nextSibling();
      }
    }
  }
  QDomElement
  operator>>= (const ScanDescriptionIDL& _lhs, QDomNode& _node)
  {
    QDomDocument d = _node.ownerDocument();
    QDomElement e = d.createElement("parameter");
    _node.appendChild(e);

    QDomElement g;

    g = (_lhs.eventName >>= e);
    g.setAttribute("name", "EventName");
    g = (_lhs.group >>= e);
    g.setAttribute("name", "Group");
    g = (_lhs.scanType >>= e);
    g.setAttribute("name", "ScanType");

    return e;
  }
#endif
