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

#include "miro/SphinxSpeechC.h"
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
    SphinxSpeech_var speech(client.resolveName<SphinxSpeech>("Speech"));

    while(true) {
      cout << "Speech test!" << endl
	   << "Chose Command: " << endl
	   << "  t - insert text" << endl
	//	   << "  v - set voice" << endl
	   << "  g - get last sentence" << endl
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
      case 'v':
	/*	cout << "Voices:" << endl
		<< " 0 - Perfect Paul" << endl
		<< " 1 - Vader" << endl
		<< " 2 - Big Bob" << endl
		<< " 3 - Precise Pete" << endl
		<< " 4 - Ricochet Randy" << endl
		<< " 5 - Biff" << endl
		<< " 6 - Skip" << endl
		<< " 7 - Robo Robert" << endl*/
	cout << "Voice Number: " << flush;
	cin >> value;
	speech->setVoice((char)value);
	break;
      case 'g': {
	SentenceIDL *sentence=speech->getLastSentence();
	if (!sentence->valid) cout << "No sentence available at the moment" << endl;
	else {
	  cout << "word   startFrame endFrame confidence aScore lScore latticeDen phonePerplex" << endl;
	  for(int i=0; i<sentence->sentence.length(); i++) {
	    cout << sentence->sentence[i].word << " "
		 << sentence->sentence[i].startFrame << " "
		 << sentence->sentence[i].endFrame << " "
		 << sentence->sentence[i].confidence << " "
		 << sentence->sentence[i].aScore << " "
		 << sentence->sentence[i].lScore << " "
		 << sentence->sentence[i].latticeDensity << " "
		 << sentence->sentence[i].phonePerplexity << endl;
	  }
	}
	break;
      }
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




