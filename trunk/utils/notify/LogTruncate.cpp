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
#include "miro/Client.h"
#include "miro/StructuredPushConsumer.h"
#include "miro/TimeHelper.h"

#include <ace/Mem_Map.h>
#include <tao/CDR.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    cout << "usage: " << argv[0] << " <list of files>" << endl
	 << "Deletes trailing zeros from a log file." << endl
	 << "This happens if logNotify wasn't able to shutdown propperly." << endl;
    return 1;
  }

  Miro::Client client(argc, argv);

  try {
    for (int i = 1; i < argc; ++i) {
      
      cout << "processing file: " << argv[i] << endl;

      cout << "memory mapping file." << endl;
      ACE_Mem_Map memoryMap(argv[i]);
      if (0 == memoryMap.handle())
	throw Miro::CException(errno, std::strerror(errno));

      TAO_InputCDR istr((char*)memoryMap.addr(), memoryMap.size());

      ACE_Time_Value timeStamp;
      Miro::TimeIDL timeIDL;
      CosNotification::StructuredEvent event;
      int counter = 0;
      int totalLength = 0;
      
      cout << "parsing cdr stream" << flush;
      while(istr.length() != 0) {
	totalLength = istr.rd_ptr() - (char*)memoryMap.addr();

	istr >> timeIDL;
	Miro::timeC2A(timeIDL, timeStamp);
	if (timeStamp == ACE_Time_Value(0))
	  break;
	istr >> event;
	if (!(counter++ % 3000))
	  cout << "." << flush;
      }
      memoryMap.close();
      cout << endl;

      if (istr.length() == 0) {
	cout << "file length == stream length" << endl;
      }
      else {
	cerr << "truncating file "<< argv[i] << " to size: " << totalLength << endl;
	if (ACE_OS::truncate(argv[i], totalLength) == -1)
	  throw Miro::CException(errno, std::strerror(errno));
      }
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
