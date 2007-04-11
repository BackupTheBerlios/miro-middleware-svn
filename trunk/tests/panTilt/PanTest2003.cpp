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
#include <miro/TimeHelper.h>

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
  bool quit = false;
  double poisition;
  double speed;
  char c, buffer[256];
  Miro::PanLimitsIDL panLimits;

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    SparrowPanTilt_var pan = client.resolveName<SparrowPanTilt>("PanTilt");

    while(!quit) {
      cout << "Pan test!" << endl
	   << "Chose Command: " << endl
	   << "  0 - get pan position" << endl
	   << "  1 - set pan position" << endl
	   << "  2 - get pan limits" << endl
	   << "  3 - set pan speed" << endl
	   << "  4 - set pan pos + speed" << endl
	   << "  5 - check if still panning" << endl
	   << " q to quit" << endl;
      cin.getline(buffer,256);
      c = buffer[0];
      if (c == 'q')
	break;

  	ACE_Time_Value now = ACE_OS::gettimeofday();
    	Miro::TimeIDL cTimeStamp;
      	Miro::timeA2C(now, cTimeStamp);


      switch (c) {
      case '0':
	cout << "current pan angle (deg): " 
	     << (Miro::rad2Deg(pan->getPan()))  
	     << " (target: " << Miro::rad2Deg(pan->getTargetPan()) << ")"
	     << endl;
	break;
      case '1':
	cout << "new panning angle (deg): " << flush;
	cin >> poisition;
	pan->setPan(Miro::deg2Rad(poisition));
	break;
      case '2':
	panLimits = pan->getPanLimits();
	cout << "pan limits: " << flush
	     << Miro::rad2Deg(panLimits.minAngle) << " <-> " << Miro::rad2Deg(panLimits.maxAngle)<<endl;
	break;	
      case '3':
        cout << "new panning speed (deg/sec): " << flush;
	cin >> speed;
	pan->setPanSpeed(Miro::deg2Rad(speed));
	break;		
	     case '4':
        cout << "new panning pos (deg): " << flush;
	cin >> poisition;
	cout << "new panning speed (deg/sec): " << flush;
  cin >> speed;
	pan->setPanExt(Miro::deg2Rad(poisition),Miro::deg2Rad(speed));
	break;		
	     case '5':
	cout << "isPanning: ";
       	if(pan->panning(cTimeStamp))
	{
		cout << "yes";
	}
	else
	{
		cout << "no";
	}
	cout << endl;
	break;		
      case 'q':
	quit = true;
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




