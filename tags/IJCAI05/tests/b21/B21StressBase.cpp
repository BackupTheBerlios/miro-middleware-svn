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

#include "idl/B21MotionC.h"
#include "miro/Client.h"
#include "miro/Task.h"

#include "miro/Synch.h"
#include "miro/TimeHelper.h"

#include <iostream>
#include <cmath>

using namespace Miro;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int main(int argc, char *argv[])
{
  int j = 300;
  
  if (argc >=2)
    j = atoi(argv[1]);

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    B21Motion_var motion = client.resolveName<B21Motion>("Motion");

    cout << "Stressing Base by sending " << j << " commands:" << endl;
    ACE_Time_Value t1 = ACE_OS::gettimeofday();

    cout << "Start time: " << t1 << endl;

    for (int i=0; i < j; ++i) {

      motion->setRotateVelocity(M_PI/6);
      motion->setRotateVelocity(M_PI/12);

    }
    ACE_Time_Value t2 = ACE_OS::gettimeofday();

    cout << "Ending stress test." << endl;
    cout << "End time: " << t2 << endl;
    cout << "Elapsed time: " << t2 - t1 << endl;
  }
  catch (const Miro::EOutOfBounds & e) {
    cerr << "Out of bounds exception on server." << endl;
    return 1;
  }
  catch (const Miro::EDevIO & e) {
    cerr << "Device IO exception on server." << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}




