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
#include "idl/RangeSensorC.h"
#include "miro/ScanDescriptionHelper.h"

using std::cout;
using std::cerr;
using std::endl;

using Miro::Client;
using Miro::RangeSensor;
using Miro::RangeSensor_var;
using Miro::ScanDescriptionIDL_var;

int main(int argc, char * argv[])
{
  Client client(argc, argv);

  if (argc < 2) {
    cout << "usage: " << argv[0] << " <sensor name>" << endl;
    return 1;
  }

  cout << "scan desecription of " << argv[1] << ":" << endl;

  try {
    RangeSensor_var sensor = client.resolveName<RangeSensor>(argv[1]);

    ScanDescriptionIDL_var description = sensor->getScanDescription();

    cout << *description << endl;
  }
  catch (const CORBA::Exception& e) {
    cerr << "CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}
