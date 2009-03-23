// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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




