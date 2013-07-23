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
#include "idl/KickerC.h"
#include "miro/Client.h"

#include <string>
#include <iostream>

using namespace Miro;
using namespace std;


int main(int argc, char * argv[])
{
  Client client(argc, argv);

  try {
    Kicker_var kicker = client.resolveName<Kicker>("Kicker");

    while (true) {
      cout << "Kicker interface test: " << endl
	   << " 0 - kick" << endl
	   << " q - quit" << endl
	   << "Choice: " << flush;
      
      string input;
      if (! (cin >> input))
	break;

      if (input[0] == 'q')
	break;
      
      if (input[0] == '0') {
	cout << "Kick duration (ms): " << flush;
	int msec;
	cin >> msec;
	Miro::TimeIDL t;
	t.sec = msec / 1000;
	t.usec = (msec % 1000) * 1000;

	cout << t.sec << ".";
	cout.width(6);
	cout.fill('0');
	cout << t.usec << endl;

	kicker->kick(t);
      }
      else {
	cout << "wrong choice!" << endl;
      }
    }
  }
  catch (CORBA::Exception const& e) {
    cerr << "CORBA exception thrown:" << e << endl;
    return 1;
  }
  return 0;
}

