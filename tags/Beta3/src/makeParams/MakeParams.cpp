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

#define QT_ALTERNATE_QTSMANIP

#include <ace/Get_Opt.h>

#include "../params/Parser.h"
#include "../params/Generator.h"
#include "../params/TextErrorHandler.h"

#include <qstring.h>
#include <qfile.h>
#include <qxml.h>

#include <iostream>
#include <fstream> 

using std::cout;
using std::cerr;
using std::ofstream;

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
      cerr << "usage: " << argv[0] << "[-f file] [-i item] [-s=source] [-h=header] [-v?]" << std::endl
	   << "  -f <file> name of the input file (Parameters.xml)" << std::endl
	   << "  -n <name> base name of the output file (Parameters)" << std::endl
	   << "  -s <extension> extension of the generated source file (cpp)" << std::endl
	   << "  -h <extension> extension of the generated header file (h)" << std::endl
	   << "  -v verbose mode" << std::endl
	   << "  -? help: emit this text and stop" << std::endl;
      rc = 1;
    }

    if (verbose) {
      cout << "file name: " << fileName << std::endl
	   << "base name: " << baseName << std::endl
	   << "source extension: " << sourceExtension << std::endl
	   << "header extension: " << headerExtension << std::endl;
    }
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
      TextErrorHandler errorHandler;
      
      reader.setContentHandler( &handler );
      reader.setErrorHandler( &errorHandler );
      if (verbose)
	cout << "parsing " << fileName << std::endl;
      bool success = reader.parse( source );
      
      if (success) {

	if (verbose)
	  cout << "generating  " << (baseName + "." + sourceExtension) << std::endl;
	ofstream sourceFile(baseName + "." + sourceExtension);
	if (verbose)
	  cout << "generating  " << (baseName + "." + headerExtension) << std::endl;
	ofstream headerFile(baseName + "." + headerExtension);
      
	generator.generateSource(sourceFile);
	generator.generateHeader(headerFile);
      }
      else {
	cerr << "Error parsing " << fileName << std::endl
	     << errorHandler.errorString();
	rc = 1;
      }
    }
    catch (const QString& e) {
      cerr << "Exception: " << e << std::endl;
      rc = 1;
    }
  }
  return rc;
}
