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

#include "miro/Client.h"
#include "miro/StructuredPushConsumer.h"
#include "miro/TimeHelper.h"

#include <ace/Mem_Map.h>

int
main(int argc, char *argv[])
{
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
  catch (...) {
    cerr << "Uncaught exception." << endl;
    return 1;
  }
  return 0;
}
