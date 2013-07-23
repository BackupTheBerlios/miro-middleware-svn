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
