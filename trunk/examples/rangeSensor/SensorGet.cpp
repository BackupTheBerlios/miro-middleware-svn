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

#include "idl/RangeSensorC.h"
#include "idl/ExceptionC.h"
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
using Miro::EDevIO;
using Miro::EOutOfBounds;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {

    if( argc <= 2 ) {
      cout << "usage: " << argv[0] << " <SensorName> <sensor#1> <sensor#2> ..." << endl;
      cout << "prints sensor values of the specified sensor." << endl;
      return 0;
    }
    
    RangeSensor_var sensor = client.resolveName<RangeSensor>(argv[1]);

    RangeGroupEventIDL_var pRangeSensorScan;

    cerr << "press return to start..." << flush;
    getchar();
    while(true) {
      pRangeSensorScan = sensor->getGroup(0);

      for( int i = 2; i < argc ; i++ ) {
	int k;
	sscanf( argv[ i ], " %i ", &k );
	cout << pRangeSensorScan->range[k] << "\t";
      }
      cout << endl;

#ifdef WAIT_FOR_KEY
      cerr << "press return to continue..." << flush;
      getchar();
#endif
    }
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




