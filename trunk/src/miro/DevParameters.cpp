// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "DevParameters.h"

#include <qdom.h>
#include <iostream>

using std::ostream;
using std::string;

namespace Miro
{
  DevParameters::DevParameters() :
    device()
  {}

  DevParameters::~DevParameters()
  {}

  void
  DevParameters::operator <<= (const QDomNode& node)
  {
    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	if (n1.nodeName() == "device") {
	  QDomNode n2 = n1.firstChild();
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) { // the node was really an element.
	    device = t.data();
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  ostream&
  operator << (ostream& ostr, const DevParameters& desc) 
  {
    ostr << "device = " << desc.device.c_str();
    
    return ostr;
  }
};
