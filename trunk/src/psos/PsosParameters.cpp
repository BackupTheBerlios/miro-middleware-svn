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

#include "PsosParameters.h"
#include "miro/XmlParse.h"

#include <qdom.h>

#include <iostream>

namespace 
{
  using Miro::operator <<=;
};

namespace Psos
{
  Parameters::Parameters() :

    // peoplebot defaults

    angleConvFactor(0.001534),
    distConvFactor(0.485000),
    velConvFactor(1.000000),
    rangeConvFactor(1.000000),
    diffConvFactor(0.006000),
    vel2Divisor(20.000000)
  {
    // none standart tty settings
    ttyParams.baudrate = 9600;
  }

  void
  Parameters::operator <<= (const QDomNode& node)
  {
    Super::operator <<= (node);

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull()) {
	if (n1.nodeName() == "AngleConvFactor")
	  angleConvFactor <<= n1;
	else if (n1.nodeName() == "DistConvFactor")
	  distConvFactor <<= n1;
	else if (n1.nodeName() == "VelConvFactor")
	  velConvFactor <<= n1;
	else if (n1.nodeName() == "RangeConvFactor")
	  rangeConvFactor <<= n1;
	else if (n1.nodeName() == "DiffConvFactor")
	  diffConvFactor <<= n1;
	else if (n1.nodeName() == "Vel2Divisor")
	  vel2Divisor <<= n1;
	n1 = n1.nextSibling();
      }
    }
  }

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc) 
  {
    ostr << static_cast<const Parameters::Super&>(desc) << endl
	 << "AngleConvFactor: " << desc.angleConvFactor << endl
	 << "DistConvFactor: " << desc.diffConvFactor << endl
	 << "VelConvFactor: " << desc.velConvFactor << endl
	 << "RangeConvFactor: " << desc.rangeConvFactor << endl
	 << "DistConvFactor: " << desc.diffConvFactor << endl
	 << "Vel2Divisor: " << desc.vel2Divisor;

    return ostr;
  }
};
