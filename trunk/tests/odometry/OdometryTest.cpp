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

#include "miro/OdometryC.h"
#include "miro/Client.h"
#include "miro/Angle.h"
#include "miro/IO.h"

#include <iostream>
#include <string>

int 
main(int argc, char * argv[])
{
  Miro::Client client(argc, argv);

  try {
    Miro::Odometry_var odometry = client.resolveName<Miro::Odometry>("Odometry");
    Miro::PositionIDL position;
    Miro::VelocityIDL velocity;
    Miro::MotionStatusIDL status;

    long x, y;
    double theta;

    std::string input;
    while(input != "q") {

      std::cout << "Possible Commands:" << endl << endl
		<< " 0 - getPosition" << endl
		<< " 1 - getWaitPosition" << endl
		<< " 2 - setPosition" << endl
		<< " 3 - updatePosition" << endl
		<< " 4 - getVelocity" << endl
		<< " 5 - getWaitVelocity" << endl
		<< " 6 - getStatus" << endl
		<< " 7 - getWaitStatus" << endl
		<< " 8 - getRawPosition" << endl
		<< " 9 - getWaitRawPosition" << endl
		<< "q to quit" << endl << endl
		<< "Selection: " << flush;

      std::cin >> input;

      try {
	switch (input[0]) {
	case '0':
	  position = odometry->getPosition();
	  std::cout << "Current position: " << position << endl;
	  break;
	case '1':
	  position = odometry->getWaitPosition();
	  std::cout << "Current position: " << position << endl;
	  break;
	case '2':
	  std::cout << "New position: " << endl
		    << "x (mm) - " << flush;
	  std::cin >> x;
	  std::cout << "y (mm) - " << flush;
	  std::cin >> y;
	  std::cout << "theta (°) - " << flush;
	  std::cin >> theta;
	  theta = Miro::deg2Rad(theta);

	  position.point.x = x;
	  position.point.y = y;
	  position.heading = theta;
	
	  odometry->setPosition(position);
	  break;
	case '3':
	  std::cout << "Position correction: " << endl
		    << "dx (mm) - " << flush;
	  std::cin >> x;
	  std::cout << "dy (mm) - " << flush;
	  std::cin >> y;
	  std::cout << "delta (°) - " <<  flush;
	  std::cin >> theta;
	  theta = Miro::deg2Rad(theta);

	  position.point.x = x;
	  position.point.y = y;
	  position.heading = theta;
	
	  odometry->updatePosition(position);
	  break;
	case '4':
	  velocity = odometry->getVelocity();
	  std::cout << "Current velocity: " << velocity << endl;
	  break;
	case '5':
	  velocity = odometry->getWaitVelocity();
	  std::cout << "Current velocity: " << velocity << endl;
	  break;
	case '6':
	  status = odometry->getStatus();
	  std::cout << "Current motion status: " << status << endl;
	  break;
	case '7':
	  status = odometry->getWaitStatus();
	  std::cout << "Current motion status: " << status << endl;
	  break;
	case '8':
	  position = odometry->getRawPosition();
	  std::cout << "Current position: " << position << endl;
	  break;
	case '9':
	  position = odometry->getWaitRawPosition();
	  std::cout << "Current position: " << position << endl;
	  break;
	default:
	  std::cerr << "Unknown command: " << input << endl;
	  break;
	}
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
