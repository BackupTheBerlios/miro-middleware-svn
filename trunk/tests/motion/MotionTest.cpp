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

#include "MotionInterface.h"

#include "idl/MotionC.h"
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
    Miro::Motion_var motion = client.resolveName<Miro::Motion>("Motion");
    MotionInterface interface(motion.in());

    std::string input;
    while(input != "q") {

      std::cout << "Possible Commands:" << std::endl << std::endl;
      
      interface.printOptions();
      std::cout << "q to quit" << std::endl << std::endl
		<< "Selection: " << std::flush;

      std::cin >> input;
      
      try {
	interface.evalCommand(input[0]);
      }
      catch (const Miro::EOutOfBounds& e) {
	std::cerr << "ERROR: Service reported: Parameter out of bounds." << std::endl << e << std::endl;
      }
      catch (const Miro::EDevIO& e) {
	std::cerr << "ERROR: Service reported: Device IO error." << std::endl << e << std::endl;
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
