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


#include "LogNotifyParameters.h"

#include <qdom.h>

#include <iostream>
#include <strstream>

using std::ostream;
using std::cout;
using std::endl;
using std::string;

LogNotifyParameters::LogNotifyParameters() :
  maxFileSize(150*1024*1024)
{
}

void
LogNotifyParameters::operator <<= (const QDomNode& node)
{
  if (!node.isNull()) {
    QDomNode n1 = node.firstChild();
    while(!n1.isNull() ) {
      QDomNode n2 = n1.firstChild();
      if (!n2.isNull()) {
	QDomText t = n2.toText(); // try to convert the node to a text
	if(!t.isNull() ) {        // the node was really a text element.
	  if (n1.nodeName() == "maxfilesize") {
	    maxFileSize = t.data().toInt();
	  }
 	  if (n1.nodeName() == "type_name") {
 	    typeNames.push_back(string(t.data()));
 	  }
	}
      }
      n1 = n1.nextSibling();
    }
  }
}

ostream&
operator << (ostream& ostr, const LogNotifyParameters& rhs) 
{
  ostr << "max fileSize = " << rhs.maxFileSize << endl
       << "loged types: ";
  LogNotifyParameters::LogTypesVector::const_iterator i;
  for (i = rhs.typeNames.begin(); i != rhs.typeNames.end(); ++i)
    cout << endl << *i;
    
  return ostr;
}

