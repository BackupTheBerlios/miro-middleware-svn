// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/RangeSensorC.h"
#include "miro/Client.h"

#include <iostream>

#include <stdio.h>

#define WAIT_FOR_KEY

using std::cout;
using std::cerr;

using Miro::Client;
using Miro::RangeSensor;
using Miro::RangeSensor_var;
using Miro::RangeGroupEventIDL_var;
using Miro::ETimeOut;
using Miro::EDevIO;
using Miro::EOutOfBounds;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {

    if( argc <= 1 ) {
      cout << "usage: <angle#1> <angle#2> ..." << endl;
      cout << "prints laservalues of the specified angles." << endl;
      return 0;
    }
    
    RangeSensor_var laser = client.resolveName<RangeSensor>("Laser");

    RangeGroupEventIDL_var pLaserScan;

    cerr << "press return to start..." << flush;
    getchar();
    while(true) {
      pLaserScan = laser->getWaitGroup(0);

      for( int i = 1; i < argc; i++ ) {
	int k;
	sscanf( argv[ i ], " %i ", &k );
	cout << pLaserScan->range[k] << "\t";
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
  return 0;
}




