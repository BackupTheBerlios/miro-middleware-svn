// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "BaseParameters.h"

#include "miro/Angle.h"

#include <qdom.h>

#include <iostream>
#include <cmath>

namespace Base
{
  Miro::Singleton<Parameters> Parameters::instance;
  
  Parameters::Parameters() :
    radius(26.7),    // cm - overall
    // reduction - 3:1*72:12 = 18.00:1
    // 500 encoder counts/motor turn
    // 4.4 inch wheel
    maxTransVelocity(1000),    // maximum speed (mm /  s)
    maxTransAccel(2000),       // maximum acceleration (mm / (s*s))
    maxRotVelocity(M_PI*2./3.), // maximum speed (rad /  s)
    maxRotAccel(M_PI),           // maximum acceleration (rad / (s*s))
    voltWarn(44),               // warning voltage
    voltPanic(39)               // panic voltage
  {
    device = "/dev/cur0";
  }

  Parameters::~Parameters()
  {}

  void
  Parameters::operator <<= (const QDomNode& node)
  {
    Super::operator <<= (node);

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomNode n2 = n1.firstChild();
	if (!n2.isNull()) {
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) {        // the node was really a text element.
	    if (n1.nodeName() == "radius") {
	      radius = t.data().toDouble();
	    }
	    if (n1.nodeName() == "maxTransVelocity") {
	      maxTransVelocity = t.data().toInt();
	    }
	    if (n1.nodeName() == "maxTransAccel") {
	      maxTransAccel = t.data().toInt();
	    }
	    if (n1.nodeName() == "maxRotVelocity") {
	      maxRotVelocity = Miro::deg2Rad(t.data().toDouble());
	    }
	    if (n1.nodeName() == "maxRotAccel") {
	       maxRotAccel = Miro::deg2Rad(t.data().toDouble());
	    }
	    if (n1.nodeName() == "voltWarn") {
	      voltWarn = t.data().toInt();
	    }
	    if (n1.nodeName() == "voltPanic") {
	      voltPanic = t.data().toInt();
	    }
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc) 
  {
    ostr << static_cast<const Parameters::Super&>(desc) << endl

	 << "radius = " << desc.radius << endl
    
	 << "maxTransVelocity = " << desc.maxTransVelocity << endl
	 << "maxTransAccel = " << desc.maxTransAccel << endl
	 << "maxRotVelocity = " << desc.maxRotVelocity << endl
	 << "maxRotAccel = " << desc.maxRotAccel << endl
    
	 << "voltWarn = " << desc.voltWarn << endl
	 << "voltPanic = " << desc.voltPanic;
    
    return ostr;
  }
};
