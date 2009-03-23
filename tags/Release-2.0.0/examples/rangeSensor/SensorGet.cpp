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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#include "idl/RangeSensorC.h"
#include "idl/ExceptionC.h"
#include "miro/Client.h"

#include <iostream>
#include <stdio.h>

#define WAIT_FOR_KEY

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using Miro::Client;
using Miro::RangeSensor;
using Miro::RangeSensor_var;
using Miro::RangeGroupEventIDL_var;
using Miro::EDevIO;
using Miro::EOutOfBounds;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {

    if( argc <= 2 ) {
      cout << "usage: " << argv[0] << " <SensorName> <sensor#1> <sensor#2> ..." << endl;
      cout << "prints sensor values of the specified sensor." << endl;
      return 0;
    }
    
    RangeSensor_var sensor = client.resolveName<RangeSensor>(argv[1]);

    RangeGroupEventIDL_var pRangeSensorScan;

    cerr << "press return to start..." << flush;
    getchar();
    while(true) {
      pRangeSensorScan = sensor->getGroup(0);

      for( int i = 2; i < argc ; i++ ) {
	int k;
	sscanf( argv[ i ], " %i ", &k );
	cout << pRangeSensorScan->range[k] << "\t";
      }
      cout << endl;

#ifdef WAIT_FOR_KEY
      cerr << "press return to continue..." << flush;
      getchar();
#endif
    }
  }
  catch (const EDevIO & e) {
    cerr << "Miro Device I/O exception: " << endl;
    return 1;
  }
  catch (const EOutOfBounds & e) {
    cerr << "Miro out of bounds exception: " << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}




