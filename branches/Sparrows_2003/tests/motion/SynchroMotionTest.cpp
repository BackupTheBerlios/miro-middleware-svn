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

#include "SynchroMotionInterface.h"

#include "miro/SynchroMotionC.h"
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
    Miro::SynchroMotion_var motion = client.resolveName<Miro::SynchroMotion>("Motion");
    SynchroMotionInterface interface(motion.in());

    std::string input;
    while(input != "q") {

      std::cout << "Possible Commands:" << endl << endl;
      
      interface.printOptions();
      std::cout << "q to quit" << endl << endl
		<< "Selection: " << flush;

      std::cin >> input;

      try {
	interface.evalCommand(input[0]);
      }
      catch (const Miro::EOutOfBounds& e) {
	std::cerr << "ERROR: Service reported: Parameter out of bounds." << endl << e << endl;
      }
      catch (const Miro::EDevIO& e) {
	std::cerr << "ERROR: Service reported: Device IO error." << endl << e << endl;
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
