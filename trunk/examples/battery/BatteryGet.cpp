// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#include "miro/Client.h"
#include "idl/BatteryC.h"

#include <iostream>

using std::cout;
using std::cerr;
using Miro::Client;
using Miro::Battery;
using Miro::Battery_var;

int main(int argc, char *argv[])
{
  Client client(argc, argv);            // Initialize orb.
  try {                                 // Obtain reference to battery object.
    Battery_var battery = client.resolveName<Battery>("Battery");
    double voltage;
  
    voltage = battery->getVoltage();   // Get the current battery voltage.

    cout << "Battery voltage: " << voltage << endl;
  }
  catch (const CORBA::Exception& e) {   // Catch any CORBA exception.
    cerr << "Exception on sonar query:" << endl << e << endl;
    return 1;
  }
  
  return 0;
}
