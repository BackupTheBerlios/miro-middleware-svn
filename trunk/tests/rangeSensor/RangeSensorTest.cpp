// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "miro/RangeSensorC.h"
#include "miro/Client.h"
#include "miro/ScanDescriptionHelper.h"
#include "miro/IO.h"

#include <iostream>
#include <string>

int 
main(int argc, char * argv[])
{
  Miro::Client client(argc, argv);

  try {
    Miro::RangeSensor_var rangeSensor = client.resolveName<Miro::RangeSensor>("RangeSensor");
    Miro::ScanDescriptionIDL_var description;
    Miro::RangeGroupEventIDL_var group;
    Miro::RangeScanEventIDL_var scan;

    CORBA::ULong id;

    std::string input;
    while(input != "q") {

      std::cout << "Possible Commands:" << endl << endl
		<< " 0 - getScanDescription" << endl
		<< " 1 - getGroup" << endl
		<< " 2 - getWaitGroup" << endl
		<< " 3 - getFullScan" << endl
		<< " 4 - getWaitFullScan" << endl
		<< "q to quit" << endl << endl
		<< "Selection: " << flush;

      std::cin >> input;

      try {
	switch (input[0]) {
	case '0':
	  description = rangeSensor->getScanDescription();
	  std::cout << "Scan description: " << endl << *description << endl;
	  break;
	case '1':
	  std::cout << "Group id: " << flush;
	  std::cin >> id;
	  group = rangeSensor->getGroup(id);
	  std::cout << "Scan of group " << id << ": " << endl << *group << endl;
	  break;
	case '2':
	  std::cout << "Group id: " << flush;
	  std::cin >> id;
	  group = rangeSensor->getWaitGroup(id);
	  std::cout << "Scan of group " << id << ": " << endl << *group << endl;
	  break;
	case '3':
	  scan = rangeSensor->getFullScan();
	  std::cout << "Full scan:" << endl << *scan << endl;
	  break;
	case '4':
	  scan = rangeSensor->getWaitFullScan();
	  std::cout << "Full scan:" << endl << *scan << endl;
	  break;
	default:
	  std::cerr << "Unknown command: " << input << endl;
	  break;
	}
      }
      catch (const Miro::EOutOfBounds& e) {
	std::cerr << "ERROR: Service reported: Parameter out of bounds." << endl << e << endl;
      }
      catch (const Miro::ETimeOut& e) {
	std::cerr << "ERROR: Service reported: Timeout." << endl << e << endl;
      }
    }
  }
  catch (const CORBA::Exception& e) {
    std::cerr << "CORBA exception occured: " << endl
	      << e << endl;
    return 1;
  }

  return 0;
}
