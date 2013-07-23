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
#include "idl/OdometryC.h"
#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

using Miro::Client;
using Miro::Odometry;
using Miro::Odometry_var;
using Miro::ETimeOut;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
    // Reference to the server object
    Odometry_var odometry = client.resolveName<Odometry>("Odometry");

    while(true) {
      try {
	cout << odometry->getWaitStatus() << endl;
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




