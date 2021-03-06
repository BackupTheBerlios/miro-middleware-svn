// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#include "miro/ButtonsC.h"
#include "miro/Client.h"

using namespace Miro;

int 
main(int argc, char* argv[])
{
  int rc = 0;
  Client client(argc, argv);
  
  try {
    Buttons_var buttons = client.resolveName<Buttons>("Buttons");

    int number = buttons->numberButtons();

    while(1) {
      cout << "Buttons status: ";
      for (int i = 0; i < number; ++i) {
	bool pressed = buttons->isPressed(i);
	cout << i << ": " << ((pressed)? "pressed   " : "unpressed ") << flush;
      }
      cout << endl;

      ACE_OS::sleep(ACE_Time_Value(0, 100000));
    }
  }
  catch (const CORBA::Exception& e) {
    cout << "CORBA exception occured: " << e << endl;
    rc = 1;
  }
  catch (...) {
    cout << "Cought unknown exception!" << endl;
    rc = 1;
  }

  return rc;
}

