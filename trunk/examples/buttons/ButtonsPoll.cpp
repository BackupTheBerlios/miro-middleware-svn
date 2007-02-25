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
#include "idl/ButtonsC.h"
#include "miro/Client.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using Miro::Client;
using Miro::Buttons;
using Miro::Buttons_var;

int 
main(int argc, char* argv[])
{
  int rc = 0;
  Client client(argc, argv);
  
  try {
    Buttons_var buttons = client.resolveName<Buttons>("Buttons");

    int number = buttons->numberOfButtons();

    while(true) {
      cout << "Buttons status: ";
      for (int i = 0; i < number; ++i) {
	bool pressed = buttons->isPressed(i);
	cout << i << ": " << ((pressed)? "pressed   " : "unpressed ") << "\t" << flush;
      }
      cout << endl;

      ACE_OS::sleep(ACE_Time_Value(0, 100000));
    }
  }
  catch (const CORBA::Exception& e) {
    cerr << "CORBA exception occured: " << e << endl;
    rc = 1;
  }

  return rc;
}

