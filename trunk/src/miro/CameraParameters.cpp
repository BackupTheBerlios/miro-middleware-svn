// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "CameraParameters.h"
#include "Angle.h"
#include "TimeHelper.h"

#include <qdom.h>
#include <iostream>

namespace Miro
{
  Singleton<CameraParameters> CameraParameters::instance;

  CameraParameters::CameraParameters() :
    ncx(768.0),
    nfx(384.0),
    dx(0.00625),
    dy(2 * dx),
    cx(0.),
    cy(0.),
    sx(0.),
    f(0.),
    kappa(0.),
    height(0.),
    alpha(0.),
    latency(0, 120000)
  {}

  void
  CameraParameters::operator <<= (const QDomNode& node)
  {
    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	if (n1.nodeName() == "ncx") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    ncx = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "nfx") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    nfx = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "dx") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    dx = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "dy") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    dy = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "cx") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    cx = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "cy") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    cy = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "sx") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    sx = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "f") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    f = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "kappa") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    kappa = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "height") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    height = t.data().toDouble();
	  }
	}
	else if (n1.nodeName() == "alpha") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    alpha = deg2Rad(t.data().toDouble());
	  }
	}
	else if (n1.nodeName() == "latency") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    double d = t.data().toDouble();
	    latency.sec((int)floor(d));
	    latency.usec((int)floor((d - floor(d)) * 1000000.));
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  std::ostream&
  operator << (std::ostream& ostr, const CameraParameters& desc) 
  {
    ostr << "ncx = " << desc.ncx << endl
	 << "nfx = " << desc.nfx << endl
	 << "dx = " << desc.dx << endl
	 << "dy = " << desc.dy << endl
	 << "cx = " << desc.cx << endl
	 << "cy = " << desc.cy << endl
	 << "sx = " << desc.sx << endl
	 << "f = " << desc.f << endl
	 << "kappa = " << desc.kappa << endl
	 << "height = " << desc.height << endl
	 << "alpha = " << rad2Deg(desc.alpha) << endl
	 << "latency = " << desc.latency << endl;
    
    return ostr;
  }
};
