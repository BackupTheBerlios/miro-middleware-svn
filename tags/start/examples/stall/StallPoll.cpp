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

#include "miro/StallC.h"
#include "miro/Client.h"

#include <iostream>

using Miro::Client;
using Miro::Stall;
using Miro::Stall_var;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);
  try {
    Stall_var stall = client.resolveName<Stall>("Stall");

    while(true) {
      if (stall->isStalled())
	cout << "stall detected" << endl;
      ACE_OS::sleep(ACE_Time_Value(0, 300000));
    }
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  catch (...) {
    cerr << "Uncaught exception: " << endl;
    return 1;
  }
  return 0;
}




