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

#include "miro/OdometryC.h"

#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>

using namespace Miro;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
    // Reference to the server object
    Odometry_var motion = client.resolveName<Odometry>("Odometry");

    while(true) {
      try {
	cout << motion->getWaitStatus() << endl;
      }
      catch (const ETimeOut& e) {
	cerr << "Blocking call timed out." << e << endl;
      }
    }
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




