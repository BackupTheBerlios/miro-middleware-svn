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


#include "Parser.h"
#include "Generator.h"

#include <qfile.h>
#include <qxml.h>

#include <iostream>

int 
main (int argc, char * argv[])
{
  try {

    for ( int i=1; i < argc; i++ ) {
      Parser handler;

      QFile xmlFile( argv[i] );
      QXmlInputSource source( xmlFile );

      QXmlSimpleReader reader;
      reader.setContentHandler( &handler );
      reader.parse( source );
    }    

#ifdef SIMPLE_TEST
    Generator g;
    Parameter p("int", "name", "1", "", "");
    Parameter p2("double", "alpha", "", "°", "Distortion angle");
    
    g.setFileName("TestFile");
    g.setName("Test");
    g.setParent("Parent");
    g.addInclude("qxml.hh");
    g.addinclude("parent.hh");
    g.addParameter(p);
    g.addParameter(p2);

    g.generateHeader();
    cout << endl << endl << endl;
    g.generateSource();


    Generator g2;
    
    g2.setFileName("TestFile2");
    g2.setNamespace("Space");
    g2.setName("Test2");
    g2.addInclude("ostream");

    cout << endl << endl << endl;
    g2.generateHeader();
    cout << endl << endl << endl;
    g2.generateSource();
#endif
  }
  catch (const QString& e) {
    cerr << "Exception: " << e << endl;
    return 1;
  }
  return 0;
}
