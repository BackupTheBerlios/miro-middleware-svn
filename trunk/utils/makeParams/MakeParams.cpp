// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#define QT_GENUINE_STR
#define QT_ALTERNATE_QTSMANIP

#include <ace/Get_Opt.h>

#include "Parser.h"
#include "Generator.h"

#include "miro/Exception.h"

#include <qstring.h>
#include <qfile.h>
#include <qxml.h>

#include <iostream>
#include <fstream> 

using std::cout;
using std::cerr;

bool verbose = false;

QString baseName = "Parameters";
QString headerExtension = "h";
QString sourceExtension = "cpp";
QString fileName = "Parameters.xml";

int 
parseArgs(int& argc, char* argv[])
{
  int rc = 0;
  int c;

  ACE_Get_Opt get_opts (argc, argv, "f:h:n:s:v?");
  
  while ((c = get_opts()) != -1) {
    switch (c) {
    case 'f':
      fileName = get_opts.optarg;
      break;
    case 'h':
      headerExtension = get_opts.optarg;
      break;
    case 'n':
      baseName = get_opts.optarg;
      break;
    case 's':
      sourceExtension = get_opts.optarg;
      break;
    case 'v':
      verbose = true;
      break;
    case '?':
    default:
      cerr << "usage: " << argv[0] << "[-f file] [-i item] [-s=source] [-h=header] [-v?]" << endl
	   << "  -f <file> name of the input file (Parameters.xml)" << endl
	   << "  -f <name> base name of the output file (Parameters)" << endl
	   << "  -s <extension> extension of the generated source file (.h)" << endl
	   << "  -h <extension> extension of the generated header file (.cpp)" << endl
	   << "  -v verbose mode" << endl
	   << "  -? help: emit this text and stop" << endl;
      rc = 1;
    }

    if (verbose) 
      cout << "file name: " << fileName << endl
	   << "base name: " << baseName << endl
	   << "source extension: " << sourceExtension << endl
	   << "header extension: " << headerExtension << endl;
  }
  return rc;
}

int 
main (int argc, char * argv[])
{
  int rc;
  if ((rc = parseArgs(argc, argv)) == 0) {
    try {
      Generator generator;
      generator.setBaseName(baseName);
      generator.setHeaderExtension(headerExtension);

      Parser handler(generator);
      
      QFile xmlFile( fileName );
      QXmlInputSource source( xmlFile );
      QXmlSimpleReader reader;
      
      reader.setContentHandler( &handler );
      if (verbose)
	cout << "parsing " << fileName << endl;
      reader.parse( source );
      
      if (verbose)
	cout << "generating  " << (baseName + "." + sourceExtension) << endl;
      ofstream sourceFile(baseName + "." + sourceExtension);
      if (verbose)
	cout << "generating  " << (baseName + "." + headerExtension) << endl;
      ofstream headerFile(baseName + "." + headerExtension);
      
      generator.generateSource(sourceFile);
      generator.generateHeader(headerFile);
    }
    catch (const Miro::Exception& e) {
      cerr << "Exception: " << e << endl;
      rc = 1;
    }
  }
  return rc;
}
