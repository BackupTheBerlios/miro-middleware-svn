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




