// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#include "idl/SphinxSpeechC.h"
#include "miro/Client.h"

#include <iostream>

using namespace Miro;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

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
	  for(unsigned int i=0; i<sentence->sentence.length(); i++) {
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




