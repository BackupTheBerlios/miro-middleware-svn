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
#include "miro/Angle.h"

#include <iostream>

using namespace Miro;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  int x = 0;
  int y = 0;
  double a = 0.;

  if (argc > 1)
    x = atoi(argv[1]);
  if (argc > 2)
    y = atoi(argv[2]);
  if (argc > 3)
    a = Miro::deg2Rad(atof(argv[3]));

  cout << "x: " << x << " y: " << y << " a: " << Miro::rad2Deg(a) << endl;

  try {
    // Reference to the server object
    Odometry_var odometry = resolve_name<Odometry>(client, "Motion");

    PositionIDL data;
    data.point.x = x;
    data.point.y = y;
    data.heading = a;

    odometry->setPosition(data);

    data = odometry->getWaitPosition();
    cout << "new position" << endl;
    cout << data << endl;
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




