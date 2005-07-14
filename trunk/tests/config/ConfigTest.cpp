// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/ConfigC.h"
#include "miro/Client.h"
#include "miro/IO.h"

#include <iostream>
#include <string>

int 
main(int argc, char * argv[])
{
  Miro::Client client(argc, argv);

  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <interface name>" << std::endl;
    return 1;
  }

  try {
    Miro::Config_var config = client.resolveName<Miro::Config>(argv[1]);
    
    std::string input;
    while(input != "q") {
      
      std::cout << "Possible Commands:" << std::endl << std::endl
		<< " 0 - getParameters" << std::endl
		<< "Selection: " << std::flush;
      
      std::cin >> input;
      char * name;
      char * type;
      char * document;

      switch (input[0]) {
      case '0':
	config->getParameters(name, type, document);
	  std::cout << "Current parameters of " 
		    << name << " (" << type << "):" << std::endl
		    << document << std::endl;
	  break;
      case 'q':
	break;
      default:
	std::cerr << "Unknown command: " << input << std::endl;
	break;
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
