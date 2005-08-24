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

#include "idl/FusedRangeSensorC.h"
#include "miro/Client.h"
#include "miro/ScanDescriptionHelper.h"
#include "miro/IO.h"

#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

int 
main(int argc, char * argv[])
{
  Miro::Client client(argc, argv);

  std::string sensorName="Fused";

  if (argc >= 2) {
    sensorName=argv[1];
//     std::cout << "usage: " << argv[0] << " <Sensor Name>" << std::endl;
//     return 1;
    
  }

  try {
    Miro::FusedRangeSensor_var rangeSensor = client.resolveName<Miro::FusedRangeSensor>(sensorName.c_str());
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
		<< " 5 - addSensor" << endl
		<< " 6 - removeSensor" << endl
		<< " 7 - setInterpolationMode" << endl
		<< " 8 - setFusionMode" << endl
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
	case '5':
	  cout << "Add which sensor?" << endl;
	  cin >> input;
	  rangeSensor->addSensor(input.c_str());
	  break;
	case '6':
	  cout << "Remove which sensor?" << endl;
	  cin >> input;
	  rangeSensor->removeSensor(input.c_str());
	  break;
	case '7':
	  cout << "Choose mode (0,1)" << endl;
	  cin >> input;
	  rangeSensor->setInterpolationMode(input[0]=='0'?
				      Miro::FusedRangeSensor::MASKED:
				      Miro::FusedRangeSensor::LINEAR);
	  break;
	case '8':
	  cout << "Choose mode (0,1)" << endl;
	  cin >> input;
	  rangeSensor->setFusionMode(input[0]=='0'?
				     Miro::FusedRangeSensor::MINIMUM:
				     Miro::FusedRangeSensor::MINIMUM );
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
