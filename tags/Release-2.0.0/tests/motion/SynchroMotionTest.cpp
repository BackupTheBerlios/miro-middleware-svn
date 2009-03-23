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
#include "SynchroMotionInterface.h"

#include "idl/SynchroMotionC.h"
#include "miro/Client.h"
#include "miro/Angle.h"
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
