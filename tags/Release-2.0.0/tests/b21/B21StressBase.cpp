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
#include "idl/B21MotionC.h"
#include "miro/Client.h"
#include "miro/Task.h"

#include "miro/Synch.h"
#include "miro/TimeHelper.h"

#include <iostream>
#include <cmath>

using namespace Miro;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int main(int argc, char *argv[])
{
  int j = 300;
  
  if (argc >=2)
    j = atoi(argv[1]);

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    B21Motion_var motion = client.resolveName<B21Motion>("Motion");

    cout << "Stressing Base by sending " << j << " commands:" << endl;
    ACE_Time_Value t1 = ACE_OS::gettimeofday();

    cout << "Start time: " << t1 << endl;

    for (int i=0; i < j; ++i) {

      motion->setRotateVelocity(M_PI/6);
      motion->setRotateVelocity(M_PI/12);

    }
    ACE_Time_Value t2 = ACE_OS::gettimeofday();

    cout << "Ending stress test." << endl;
    cout << "End time: " << t2 << endl;
    cout << "Elapsed time: " << t2 - t1 << endl;
  }
  catch (const Miro::EOutOfBounds & e) {
    cerr << "Out of bounds exception on server." << endl;
    return 1;
  }
  catch (const Miro::EDevIO & e) {
    cerr << "Device IO exception on server." << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}




