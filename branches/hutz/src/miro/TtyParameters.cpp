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


#include "TtyParameters.h"

#include <qdom.h>

#include <iostream>

using std::ostream;
using std::string;

namespace Miro
{
  TtyParameters::TtyParameters()
  {
    device = "/dev/tty/0";

    // Common params
    ttyParams.baudrate = 38400;
    ttyParams.parityenb = 0;
    ttyParams.paritymode = 0;
    ttyParams.databits = 8;
    ttyParams.stopbits = 1; 
    ttyParams.readtimeoutmsec = 0;
    ttyParams.modem = 0;
    ttyParams.rcvenb = 1;
    ttyParams.ctsenb = 0;   // CTS & RTS are the same under unix
    ttyParams.rtsenb = 0;   // enable & set rts mode (win32)
    ttyParams.xinenb = 0;   // enable xon/xoff  reception
    ttyParams.xoutenb = 0;  // enable xon/xoff transmission
  }

  TtyParameters::~TtyParameters()
  {}


  void
  TtyParameters::operator <<= (const QDomNode& node)
  {
    Super::operator <<= (node);

    if (!node.isNull()) {
      QDomNode n1 = node.firstChild();
      while(!n1.isNull() ) {
	QDomNode n2 = n1.firstChild();
	if (!n2.isNull()) {
	  QDomText t = n2.toText(); // try to convert the node to a text
	  if(!t.isNull() ) {       // the node was really a text element.
	    if (n1.nodeName() == "baudrate") {
	      ttyParams.baudrate = t.data().toInt();
	    }
	    else if (n1.nodeName() == "parityenb") {
	      ttyParams.parityenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "databits") {
	      ttyParams.databits = t.data().toInt();
	    }
	    else if (n1.nodeName() == "stopbits") {
	      ttyParams.stopbits = t.data().toInt();
	    }
	    else if (n1.nodeName() == "readtimeoutmsec") {
	      ttyParams.readtimeoutmsec = t.data().toInt();
	    }
	    else if (n1.nodeName() == "modem") {
	      ttyParams.modem = t.data().toInt();
	    }
	    else if (n1.nodeName() == "rcvenb") {
	      ttyParams.rcvenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "ctsenb") {
	      ttyParams.ctsenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "rtsenb") {
	      ttyParams.rtsenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "xinenb") {
	      ttyParams.xinenb = t.data().toInt();
	    }
	    else if (n1.nodeName() == "xoutenb") {
	      ttyParams.xoutenb = t.data().toInt();
	    }
	  }
	}
	n1 = n1.nextSibling();
      }
    }
  }

  ostream&
  operator << (ostream& ostr, const TtyParameters& desc) 
  {
    ostr << static_cast<const TtyParameters::Super&>(desc) << endl

	 << "baudrate = " << desc.ttyParams.baudrate << endl
	 << "parityenb = " << desc.ttyParams.parityenb << endl
	 << "databits = " << desc.ttyParams.databits << endl
	 << "stopbits = " << desc.ttyParams.stopbits << endl 
	 << "readtimeoutmsec = " << desc.ttyParams.readtimeoutmsec << endl
	 << "modem = " << desc.ttyParams.modem << endl
	 << "rcvenb = " << desc.ttyParams.rcvenb << endl
	 << "ctsenb = " << desc.ttyParams.ctsenb << endl // CTS & RTS are the same under unix
	 << "rtsenb = " << desc.ttyParams.rtsenb << endl // enable & set rts mode (win32)
	 << "xinenb = " << desc.ttyParams.xinenb << endl // enable xon/xoff  reception
	 << "xoutenb = " << desc.ttyParams.xoutenb;       // enable xon/xoff transmission

    return ostr;
  }
};
