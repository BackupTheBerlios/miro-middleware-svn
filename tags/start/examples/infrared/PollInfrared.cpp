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

#include "miro/RangeSensorC.h"
#include "miro/Client.h"

#include <iostream>

using namespace Miro;

int main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);
  try {
    RangeSensor_var ir = client.resolveName<RangeSensor>("Infrared");

    RangeScanEventIDL_var data;
    
    ACE_Time_Value start(ACE_OS::gettimeofday());

    for (int i = 0; i < 100; ++i) {
      data = ir->getWaitFullScan();
//       cout << "Ir reading: ";
//       cout << data << endl;
    }

    start = ACE_OS::gettimeofday() - start;
    cout << "Time for 100 readings :" << start.msec() << endl;
  }
  catch (const ETimeOut & e) {
    cerr << "Miro Timeout exception: " << endl;
    return 1;
  }
  catch (const EDevIO & e) {
    cerr << "Miro Device I/O exception: " << endl;
    return 1;
  }
  catch (const EOutOfBounds & e) {
    cerr << "Miro out of bounds exception: " << endl;
    return 1;
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




