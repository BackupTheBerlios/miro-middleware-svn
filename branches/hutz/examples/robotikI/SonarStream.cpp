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

#include "miro/RangeSensorC.h"

#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

//#define WAIT_FOR_KEY

using namespace Miro;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {

    if( argc <= 1 ) {
      cout << "usage: <sonar#1> <sonar#2> ..." << endl;
      cout << "prints sonarvalues of the specified sonars." << endl;
      return 0;
    }
    
    RangeSensor_var sonar = client.resolveName<RangeSensor>("Sonar");

    RangeScanEventIDL_var pSonarScan;

    cerr << "press return to start..." << flush;
    getchar();
    while(true) {
      pSonarScan = sonar->getWaitFullScan();

      for( int i = 1; i < argc; i++ ) {
	int k;
	sscanf( argv[ i ], " %i ", &k );
	cout << pSonarScan->range[0][k] << "\t";
      }
      cout << endl;

#ifdef WAIT_FOR_KEY
      cerr << "press return to continue..." << flush;
      getchar();
#endif
    }
  }
  catch (const ETimeOut & e) {
    cerr << "Miro Timeout exception: " << endl;
    return 1;
  }
  catch (const EDevIO & e) {
    cerr << "Miro Device I/O exception: " << endl;
    return 1;
  }
  catch (const EOutOfBounds & e) {
    cerr << "Miro out of bounds exception: " << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }
  return 0;
}




