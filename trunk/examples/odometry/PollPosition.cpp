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
    Odometry_var odometry = resolve_name<Odometry>(client, "Motion");

    PositionIDL data;

    while(true) {
      try {

	// output position 10 times //
	for (int i=0; true /* i<10 */ ; i++) {
	  data = odometry->getWaitPosition();
	  cout << data << endl;
	}

	// set new position //
	int x, y;
	float h;
	cout << "x = "; cin >> x;
	cout << "y = "; cin >> y;
	cout << "heading (rad) = "; cin >> h;
	data.point.x = x;
	data.point.y = y;
	data.heading = h;
	odometry->setPosition(data);
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




