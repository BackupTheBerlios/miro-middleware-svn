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
#include "miro/Client.h"
#include "miro/IO.h"
#include "idl/TCM2C.h"

#include <iostream>
#include <stdio.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using Miro::Client;
using Miro::TCM2;
using Miro::TCM2_var;
using Miro::TCM2EventIDL;
using Miro::ETimeOut;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
    TCM2_var tcm2 = client.resolveName<TCM2>("TCM2");

    while (1) {
      TCM2EventIDL tcm2data = tcm2->getWaitData();
      cout << tcm2data << endl;
    }
  }
  catch (ETimeOut) {
    cerr << "Timeout waiting for TCM2 event.\n";
    return 1;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}

