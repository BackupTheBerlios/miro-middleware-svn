// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/RangeSensorC.h"
#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>

using std::cout;
using std::cerr;

using Miro::Client;
using Miro::RangeSensor;
using Miro::RangeSensor_var;
using Miro::RangeScanEventIDL;
using Miro::RangeScanEventIDL_var;
using Miro::EDevIO;

int main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);
  try {
    RangeSensor_var ir = client.resolveName<RangeSensor>("Infrared");

    RangeScanEventIDL * data = NULL;
    
    ACE_Time_Value start(ACE_OS::gettimeofday());

    for (int i = 0; i < 100; ++i) {
      data = ir->getWaitFullScan();
      cout << "Ir reading: " << endl;
      cout << *data << endl;
      delete data;
    }

    start = ACE_OS::gettimeofday() - start;
    cout << "Time for 100 readings :" << start.msec() << endl;
  }
  catch (const EDevIO & e) {
    cerr << "Miro Device I/O exception: " << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}




