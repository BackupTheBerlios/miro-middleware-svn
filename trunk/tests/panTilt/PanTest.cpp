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

#include "idl/PanC.h"
#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>
#include <cmath>

using Miro::Client;
using Miro::Pan;
using Miro::Pan_var;

using namespace std;

int main(int argc, char *argv[])
{
  int rc = 0;
  bool quit = false;
  double poisition;
  char c, buffer[256];

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    Pan_var pan = client.resolveName<Pan>("PanTilt");

    while(!quit) {
      cout << "Pan test!" << endl
	   << "Chose Command: " << endl
	   << "  0 - get pan position" << endl
	   << "  1 - set pan position" << endl
	   << " q to quit" << endl;
      cin.getline(buffer,256);
      c = buffer[0];
      if (c == 'q')
	break;

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
	poisition *= M_PI/180.;
	pan->setPan(poisition);
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




