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
#include <idl/SparrowPanTiltC.h>
#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>
#include <cmath>

using Miro::Client;
using Miro::SparrowPanTilt;
using Miro::SparrowPanTilt_var;
using Miro::PanTiltPositionIDL;

using namespace std;

int main(int argc, char *argv[])
{
  int rc = 0;
  ACE_Time_Value t(0);
  int msec = 0;
  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    SparrowPanTilt_var pan = client.resolveName<SparrowPanTilt>("PanTilt");
    cout << "insert interval time (ms): " << std::flush;
    t.msec(msec);
    while(1){
    cout << "current pan angle (deg): " 
	     << (Miro::rad2Deg(pan->getPan()))  
	     << " (target: " << Miro::rad2Deg(pan->getTargetPan()) << ")"
	     << endl << std::flush;
    ACE_OS::sleep(t);
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




