// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/KickerC.h"
#include "miro/Client.h"

#include <string>
#include <iostream>

using namespace Miro;
using namespace std;


int main(int argc, char * argv[])
{
  Client client(argc, argv);

  try {
    Kicker_var kicker = client.resolveName<Kicker>("Kicker");

    while (true) {
      cout << "Kicker interface test: " << endl
	   << " 0 - kick" << endl
	   << " q - quit" << endl
	   << "Choice: " << flush;
      
      string input;
      if (! (cin >> input))
	break;

      if (input[0] == 'q')
	break;
      
      if (input[0] == '0') {
	cout << "Kick duration (ms): " << flush;
	int msec;
	cin >> msec;
	Miro::TimeIDL t;
	t.sec = msec / 1000;
	t.usec = (msec % 1000) * 1000;

	cout << t.sec << ".";
	cout.width(6);
	cout.fill('0');
	cout << t.usec << endl;

	kicker->kick(t);
      }
      else {
	cout << "wrong choice!" << endl;
      }
    }
  }
  catch (CORBA::Exception const& e) {
    cerr << "CORBA exception thrown:" << e << endl;
    return 1;
  }
  return 0;
}

