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

#include "idl/PanTiltC.h"
#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>
#include <cmath>

using Miro::Client;
using Miro::PanTilt;
using Miro::PanTilt_var;
using Miro::PanTiltPositionIDL;

int main(int argc, char *argv[])
{
  int rc = 0;
  bool quit = false;
  double pan, tilt;
  char c, buffer[256];
  PanTiltPositionIDL position;

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    PanTilt_var panTilt = client.resolveName<PanTilt>("PanTilt");

    while(!quit) {
      cout << "PanTilt test!" << endl
	   << "Chose Command: " << endl
	   << "  0 - get pan position" << endl
	   << "  1 - set pan position" << endl
	   << "  2 - get tilt position" << endl
	   << "  3 - set tilt position" << endl
	   << "  4 - get panilt position" << endl
	   << "  5 - set pantilt position" << endl
	   << " q to quit" << endl;
      cin.getline(buffer,256);
      c = buffer[0];
      if (c == 'q')
	break;

      switch (c) {
      case '0':
	cout << "current pan angle (deg): " 
	     << (panTilt->getPan() * 180. / M_PI)  << endl;
	break;
      case '1':
	cout << "new panning angle (deg): " << flush;
	cin >> pan;
	pan *= M_PI/180.;
	panTilt->setPan(pan);
	break;
      case '2':
	cout << "current tilt angle (deg): " 
	     << (panTilt->getTilt() * 180. / M_PI) << endl;
	break;
      case '3':
	cout << "new tilting angle (deg): " << flush;
	cin >> tilt;
	tilt *= M_PI/180.;
	panTilt->setTilt(tilt);
	break;
      case '4':
	cout << "current pantilt position: " 
	     << panTilt->getPosition() << endl;
	break;
      case '5':
	cout << "new panning angle (deg): " << flush;
	cin >> pan;
	position.panvalue = pan * M_PI/180.;
	cout << "new tilting angle (deg): " << flush;
	cin >> tilt;
	position.tiltvalue = tilt * M_PI/180.;
	panTilt->setPosition(position);
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




