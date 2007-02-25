// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
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

