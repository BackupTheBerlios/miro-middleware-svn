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

#include "idl/OdometryC.h"
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

      std::cout << "Possible Commands:" << std::endl << std::endl
		<< " 0 - getPosition" << std::endl
		<< " 1 - getWaitPosition" << std::endl
		<< " 2 - setPosition" << std::endl
		<< " 3 - updatePosition" << std::endl
		<< " 4 - getVelocity" << std::endl
		<< " 5 - getWaitVelocity" << std::endl
		<< " 6 - getStatus" << std::endl
		<< " 7 - getWaitStatus" << std::endl
		<< " 8 - getRawPosition" << std::endl
		<< " 9 - getWaitRawPosition" << std::endl
		<< "q to quit" << std::endl << std::endl
		<< "Selection: " << std::flush;

      std::cin >> input;

      try {
	switch (input[0]) {
	case '0':
	  position = odometry->getPosition();
	  std::cout << "Current position: " << position << std::endl;
	  break;
	case '1':
	  position = odometry->getWaitPosition();
	  std::cout << "Current position: " << position << std::endl;
	  break;
	case '2':
	  std::cout << "New position: " << std::endl
		    << "x (mm) - " << std::flush;
	  std::cin >> x;
	  std::cout << "y (mm) - " << std::flush;
	  std::cin >> y;
	  std::cout << "theta (°) - " << std::flush;
	  std::cin >> theta;
	  theta = Miro::deg2Rad(theta);

	  position.point.x = x;
	  position.point.y = y;
	  position.heading = theta;
	
	  odometry->setPosition(position);
	  break;
	case '3':
	  std::cout << "Position correction: " << std::endl
		    << "dx (mm) - " << std::flush;
	  std::cin >> x;
	  std::cout << "dy (mm) - " << std::flush;
	  std::cin >> y;
	  std::cout << "delta (°) - " <<  std::flush;
	  std::cin >> theta;
	  theta = Miro::deg2Rad(theta);

	  position.point.x = x;
	  position.point.y = y;
	  position.heading = theta;
	
	  odometry->updatePosition(position);
	  break;
	case '4':
	  velocity = odometry->getVelocity();
	  std::cout << "Current velocity: " << velocity << std::endl;
	  break;
	case '5':
	  velocity = odometry->getWaitVelocity();
	  std::cout << "Current velocity: " << velocity << std::endl;
	  break;
	case '6':
	  status = odometry->getStatus();
	  std::cout << "Current motion status: " << status << std::endl;
	  break;
	case '7':
	  status = odometry->getWaitStatus();
	  std::cout << "Current motion status: " << status << std::endl;
	  break;
	case '8':
	  position = odometry->getRawPosition();
	  std::cout << "Current position: " << position << std::endl;
	  break;
	case '9':
	  position = odometry->getWaitRawPosition();
	  std::cout << "Current position: " << position << std::endl;
	  break;
	case 'q':
	  break;
	default:
	  std::cerr << "Unknown command: " << input << std::endl;
	  break;
	}
      }
      catch (const Miro::ETimeOut& e) {
	std::cerr << "ERROR: Service reported: Timeout." << std::endl << e << std::endl;
      }
    }
  }
  catch (const CORBA::Exception& e) {
    std::cerr << "CORBA exception occured: " << std::endl
	      << e << std::endl;
    return 1;
  }

  return 0;
}
