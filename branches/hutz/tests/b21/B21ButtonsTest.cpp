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

#include "miro/B21ButtonsC.h"
#include "miro/Client.h"

#include <iostream>

using std::cout;
using std::cin;
using std::cerr;
using namespace Miro;

int main(int argc, char* argv[])
{
  int rc = 0;
  char c;
  CORBA::Long button = 0;
  B21Button::Mode mode = B21Button::OFF;

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    B21Buttons_var buttons(resolve_name<B21Buttons>(client, "Buttons"));

    while(true) {
      cout << "Buttons test!" << endl
	   << "Chose Button: " << endl
	   << "  r - red button" << endl
	   << "  y - yellow button" << endl
	   << "  g - green button" << endl
	   << "  b - blue button" << endl
	   << "  l - left kill button" << endl
	   << "  o - right (other :-) kill button" << endl
	   << "  q - to quit" << endl
	   << "enter your choice: " << flush;
      cin >> c;
      if (c == 'q')
	break;

      switch (c) {
      case 'r':
	button = B21Button::RED;
	break;
      case 'y':
	button = B21Button::YELLOW;
	break;
      case 'g':
	button = B21Button::GREEN;
	break;
      case 'b':
	button = B21Button::BLUE;
	break;
      case 'l':
	button = B21Button::LEFT_KILL;
	break;
      case 'o':
	button = B21Button::RIGHT_KILL;
	break;
      default:
	button = -1;
	cout << "unknown option" << endl;
      }

      if (button != -1) {
	cout << endl
	     << "Chose mode:" << endl
	     << "  0 - OFF" << endl
	     << "  1 - ON" << endl
	     << "  2 - FLASHING" << endl
	     << "  3 - FLASHING_TILL_PRESSED" << endl
	     << "  4 - ON_TILL_PRESSED" << endl
	     << "  5 - OFF_TILL_PRESSED" << endl
	     << "  6 - TOGGLE_ON" << endl
	     << "  7 - TOGGLE_OFF" << endl
	     << "  8 - ON_WHILE_PRESSED" << endl
	     << "  9 - OFF_WHILE_PRESSED" << endl
	     << "  r - to read button status" << endl
	     << "enter your choice: " << flush;
	cin >> c;

	if (c == 'r')
	  cout << "Button status:" << buttons->getButtonMode(button) << endl;
	else {
	  switch (c) {
	  case '0':
	    mode = B21Button::OFF;
	    break;
	  case '1':
	    mode = B21Button::ON;
	    break;
	  case '2':
	    mode = B21Button::FLASHING;
	    break;
	  case '3':
	    mode = B21Button::FLASHING_TILL_PRESSED;
	    break;
	  case '4':
	    mode = B21Button::ON_TILL_PRESSED;
	    break;
	  case '5':
	    mode = B21Button::OFF_TILL_PRESSED;
	    break;
	  case '6':
	    mode = B21Button::TOGGLE_ON;
	    break;
	  case '7':
	    mode = B21Button::TOGGLE_OFF;
	    break;
	  case '8':
	    mode = B21Button::ON_WHILE_PRESSED;
	    break;
	  case '9':
	    mode = B21Button::OFF_WHILE_PRESSED;
	    break;
	  default:
	    cout << "unknown option" << endl << endl;
	    continue;
	  }
	  buttons->setButtonMode(button, mode);
	  cout << endl;
	}
      }
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




