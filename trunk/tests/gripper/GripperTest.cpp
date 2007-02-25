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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#include "idl/GripperC.h"
#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>
#include <cmath>

using Miro::Client;
using Miro::Gripper;
using Miro::Gripper_var;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int main(int argc, char *argv[])
{
  int rc = 0;
  bool quit = false;
  short pressure;
  int time;
  char c, buffer[256];

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    Gripper_var gripper = client.resolveName<Gripper>("Gripper");

    while(!quit) {
      cout << "Gripper test!" << endl
	   << "Chose Command: " << endl
	   << "  0 - stop gripper" << endl
	   << "  1 - set grip pressure" << endl
	   << "  2 - open grip" << endl
	   << "  3 - close grip" << endl
	   << "  4 - raise gripper" << endl
	   << "  5 - lower gripper" << endl
	   << "  6 - move gripper (time)" << endl
	   << " q to quit" << endl;
      cin.getline(buffer,256);
      c = buffer[0];
      if (c == 'q')
	break;

      switch (c) {
      case '0':
	gripper->stopGrip();
	break;
      case '1':
	cout << "new grip pressure (percentage): " << flush;
	cin >> pressure;
	gripper->setGripPressure(pressure);
	break;
      case '2':
	gripper->openGrip();
	break;
      case '3':
	gripper->closeGrip();
	break;
      case '4':
	gripper->raiseGrip();
	break;
      case '5':
	gripper->lowerGrip();
	break;
      case '6':
	cout << "time (ms): " << flush;
	cin >> time;
	gripper->moveGrip(time);
	break;
      case 'q':
	quit = true;
	break;
      default:
	cout << "unknown option" << endl;
      }
    }
  }
  catch (const Miro::EOutOfBounds & e) {
    cerr << "Out of bounds exception on server." << endl;
    rc = 1;
  }
  catch (const Miro::EDevIO & e) {
    cerr << "Device IO exception on server." << endl;
    rc = 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    rc = 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    rc = 1;
  }
  return rc;
}




