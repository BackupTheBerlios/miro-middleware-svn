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

#include "idl/SpeechC.h"
#include "miro/Client.h"

#include <iostream>

using namespace Miro;

int main(int argc, char *argv[])
{
  int rc = 0;
  int value=0;
  char c, buffer[256];

  // Initialize server daemon.
  Client client(argc, argv);

  try {
     // Reference to the server object
    Speech_var speech(client.resolveName<Speech>("Speech"));

    while(true) {
      cout << "Speech test!" << endl
	   << "Chose Command: " << endl
	   << "  t - insert text" << endl
	//	   << "  c - insert command" << endl
	   << "  v - set voice" << endl
	//	   << "  s - skip word" << endl
	//	   << "  e - erase input buffer" << endl
	//	   << "  r - reset speech card" << endl
	//	   << "  g - get speech settings" << endl
	   << " q to quit" << endl;
      cin.getline(buffer,256);
      c = buffer[0];
      if (c == 'q')
	break;

      switch (c) {
      case 't':
	cout << "Text: " << flush;
	cin.getline(buffer, 255);
	speech->speak(buffer);
	break;

	/*      case 'c':
		cout << "Command: " << flush;
		cin >> c;
		cout << "Value: " << flush;
		cin >> value;
		speech->insertCommand(c, (char)value);
		break;
	*/
	
      case 'v':
	cout << "Voices:" << endl
	     << " 0 - Perfect Paul" << endl
	     << " 1 - Vader" << endl
	     << " 2 - Big Bob" << endl
	     << " 3 - Precise Pete" << endl
	     << " 4 - Ricochet Randy" << endl
	     << " 5 - Biff" << endl
	     << " 6 - Skip" << endl
	     << " 7 - Robo Robert" << endl
	     << "Voice Number: " << flush;
	cin >> value;
	speech->setVoice((char)value);
	break;
	/*	
	  case 's':
	  speech->skip();
	  break;
	  case 'e':
	  speech->clear();
	  break;
	  case 'r':
	  speech->reset();
	  break;
	  case 'g':
	  settings = speech->getSettings();
	  cout << "speech settings:" << endl
	  << "  1 - mode: " << (int)settings.mode << endl
	  << "  2 - punc_level: " << (int)settings.punc_level << endl
	  << "  3 - formant_freq: " << (int)settings.formant_freq << endl
	  << "  4 - pitch: " << (int)settings.pitch << endl
	  << "  5 - speed: " << (int)settings.speed << endl
	  << "  6 - volume: " << (int)settings.volume << endl
	  << "  7 - tone: " << (int)settings.tone << endl
	  << "  8 - expression: " << (int)settings.expression << endl
	  << "  9 - articulation: " << (int)settings.articulation << endl
	  << " 10 - reverb: " << (int)settings.reverb << endl << endl
	  << "  0 - none" << endl
	  << "select parameter to change" << endl;
	  
	  cin >> parm;
	  if (parm > 0 && parm < 11) {
	  cout << "New value: " << flush;
	  cin >> value;
	  }
	  switch(parm) {
	  case 1:
	  settings.mode = value;
	  break;
	  case 2:
	  settings.punc_level = value;
	  break;
	  case 3:
	  settings.formant_freq = value;
	  break;
	  case 4:
	  settings.pitch = value;
	  break;
	  case 5:
	  settings.speed = value;
	  break;
	  case 6:
	  settings.volume = value;
	  break;
	  case 7:
	  settings.tone = value;
	  break;
	  case 8:
	  settings.expression = value;
	  break;
	  case 9:
	  settings.articulation = value;
	  break;
	  case 10:
	  settings.reverb = value;
	  break;
	  default:
	  cout << "No parameter to be changed. " << endl;
	  }
	  break;
	*/
      default:
	cout << "unknown option" << endl;
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




