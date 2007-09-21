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
#include "miro/LogReader.h"
#include "miro/Log.h"

using namespace std;
using namespace Miro;

int
main(int argc, char *argv[])
{
  Miro::Log::init(argc, argv);

  if (argc <= 1) {
    cerr << "usage: " << argv[0] << " <list of files>" << endl
	 << "Deletes trailing zeros from a log file." << endl
	 << "This happens if logNotify wasn't able to shutdown propperly." << endl;
    return 1;
  }

  try {
    for (int i = 1; i < argc; ++i) {
      
      cout << "processing file: " << argv[i] << endl;

      LogReader logFile(argv[i], LogReader::TRUNCATE);

      cout << "parsing cdr stream - " << logFile.events() << " events" << flush;
      ACE_Time_Value t;
      
      unsigned int events = logFile.events();
      unsigned int i;
      for (i = 0; i < events; ++i) {
	if (logFile.eof()) {
	  cerr << "\nend of file before event " << i + 1 << endl;
	  break;
	}
	if (!logFile.parseTimeStamp(t)) {
	  cerr << "\nend of file parsing timestamp " << i + 1 << endl;
	  break;
	}
	if (!logFile.skipEvent()) {
	  cerr << "\nend of file parsing event " << i + 1 << endl;
	  break;
	}

	if ((i % 10000) == 0) {
	  cout << "." << flush;
	}
      }
      logFile.events(i);
      
      cout << endl;
      cout << "number of corrupted events: " << events - i << endl;
    }
  }
  catch (const Miro::Exception& e) {
    cerr << "Uncaught Miro exception: " << e << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
