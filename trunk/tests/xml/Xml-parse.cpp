// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


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
