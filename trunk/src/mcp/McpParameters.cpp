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


#include "McpParameters.h"
#include "McpDevice.h"

#include "miro/TimeHelper.h"

#include <qdom.h>

#include <iostream>

namespace Mcp
{
  Parameters::Parameters() :
    Super(),
    watchdogTimeout(WATCHDOG_INTERVAL)
  {
    // none standart tty settings
    ttyParams.baudrate = 38400;
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
	    if (n1.nodeName() == "watchdogTimeout") {
	      watchdogTimeout.msec(t.data().toInt());
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
	 << "watchdog timeout = " << desc.watchdogTimeout;

    return ostr;
  }
};
