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
#include "idl/RangeSensorC.h"
#include "miro/Client.h"
#include "miro/IO.h"

#include <ace/OS.h>

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

using Miro::Client;
using Miro::RangeSensor;
using Miro::RangeSensor_var;
using Miro::RangeScanEventIDL;
using Miro::RangeScanEventIDL_var;
using Miro::EDevIO;

int main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);
  try {
    RangeSensor_var ir = client.resolveName<RangeSensor>("Infrared");

    RangeScanEventIDL * data = NULL;
    
    ACE_Time_Value start(ACE_OS::gettimeofday());

    for (int i = 0; i < 100; ++i) {
      data = ir->getWaitFullScan();
      cout << "Ir reading: " << endl;
      cout << *data << endl;
      delete data;
    }

    start = ACE_OS::gettimeofday() - start;
    cout << "Time for 100 readings :" << start.msec() << endl;
  }
  catch (const EDevIO & e) {
    cerr << "Miro Device I/O exception: " << endl;
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}




