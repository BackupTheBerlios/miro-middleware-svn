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

#include "miro/MotionC.h"
#include "miro/OdometryC.h"

#include "miro/IO.h"
#include "miro/Client.h"

#include <iostream>

using namespace Miro;

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
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }
  return 0;
}




