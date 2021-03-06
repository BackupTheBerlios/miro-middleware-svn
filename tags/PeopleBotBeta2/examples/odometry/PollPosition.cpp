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

#include "miro/OdometryC.h"
#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>

using std::cout;
using std::cerr;

using Miro::Client;
using Miro::Odometry;
using Miro::Odometry_var;
using Miro::PositionIDL;
using Miro::ETimeOut;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
    // Reference to the server object
    Odometry_var odometry = client.resolveName<Odometry>("Odometry");

    PositionIDL data;

    while(true) {
      try {
	data = odometry->getWaitPosition();
	cout << data << endl;
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
  return 0;
}




