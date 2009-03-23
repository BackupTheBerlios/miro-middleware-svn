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
#include "miro/Parameters.h"

#include <qfile.h>
#include <qdom.h>

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int main(int argc, char * argv[])
{
  QDomDocument doc( "mydocument" );
  QFile f( "test.xml" );
  if ( !f.open( IO_ReadOnly ) )
      return 1;
  if ( !doc.setContent( &f ) ) {
      f.close();
      return 1;
  }
  f.close();

  // print out the element names of all elements that are a direct child
  // of the outermost element.
  QDomElement docElem = doc.documentElement();

  QDomNodeList l = doc.elementsByTagName("sparrow");

  Miro::TtyParameters params;

  for (unsigned int i = 0; i < l.length(); ++i) {
    QDomNode n = l.item(i);

    params <<= n;
    cout << params << endl;
  }

  return 0;
}
