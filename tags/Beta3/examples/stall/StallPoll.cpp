// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999,2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/StallC.h"
#include "miro/Client.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

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
  return 0;
}




