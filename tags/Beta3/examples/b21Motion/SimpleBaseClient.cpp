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

#include "idl/MotionC.h"
#include "idl/OdometryC.h"

#include "miro/IO.h"
#include "miro/Client.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

using Miro::Client;
using Miro::Odometry;
using Miro::Odometry_var;
using Miro::Motion;
using Miro::Motion_var;
using Miro::VelocityIDL;
using Miro::PositionIDL;
using Miro::EDevIO;
using Miro::ETimeOut;
using Miro::EOutOfBounds;

int main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
    // Reference to the server object
    Motion_var motion;
    Odometry_var odometry;

    motion = client.resolveName<Motion>("Motion");
    odometry = client.resolveName<Odometry>("Odometry");

    VelocityIDL speed;
    PositionIDL data;
    
    data = odometry->getPosition();
    cout << data << endl;

    speed.rotation = 0;
    speed.translation = 200;

    motion->setVelocity(speed);
    ACE_OS::sleep(1);

    data = odometry->getPosition();
    cout << data << endl;
    ACE_OS::sleep(1);
    
    speed = odometry->getWaitVelocity();

    cout << speed << endl;

    speed.translation = 0;
    motion->setVelocity(speed);

    data = odometry->getWaitPosition();

    cout << data << endl;    
  }
  catch (const ETimeOut & e) {
    cerr << "Miro Timeout exception: " << e << endl;
    return 1;
  }
  catch (const EDevIO & e) {
    cerr << "Miro Device I/O exception: " << e << endl;
    return 1;
  }
  catch (const EOutOfBounds & e) {
    cerr << "Miro out of bounds exception: " << e << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}




