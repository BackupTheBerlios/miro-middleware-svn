// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/Client.h"
#include "miro/IO.h"
#include "idl/TCM2C.h"

#include <iostream>
#include <stdio.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using Miro::Client;
using Miro::TCM2;
using Miro::TCM2_var;
using Miro::TCM2EventIDL;
using Miro::ETimeOut;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
    TCM2_var tcm2 = client.resolveName<TCM2>("TCM2");

    while (1) {
      TCM2EventIDL tcm2data = tcm2->getWaitData();
      cout << tcm2data << endl;
    }
  }
  catch (ETimeOut) {
    cerr << "Timeout waiting for TCM2 event.\n";
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}

