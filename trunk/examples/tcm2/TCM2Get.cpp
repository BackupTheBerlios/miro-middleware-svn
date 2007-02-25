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
#include "miro/Angle.h"
#include "idl/TCM2C.h"

#include <iostream>
#include <stdio.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

using Miro::Client;
using Miro::rad2Deg;
using Miro::TCM2;
using Miro::TCM2_var;
using Miro::TCM2EventIDL;

int
main(int argc, char *argv[])
{
  // Initialize server daemon.
  Client client(argc, argv);

  try {
    TCM2_var tcm2 = client.resolveName<TCM2>("TCM2");
    TCM2EventIDL tcm2data = tcm2->getData();

    cout << "Compass Heading: " << rad2Deg(tcm2data.heading) << " deg" << endl;
    cout << "Inclination: ";
    cout << "pitch = " << rad2Deg(tcm2data.inclination.pitch) << " deg, ";
    cout << "roll = " << rad2Deg(tcm2data.inclination.roll) << " deg" << endl;
    cout << "Magnetic field strength: ";
    cout << "x = " << tcm2data.fieldstrength.x << " uT, ";
    cout << "y = " << tcm2data.fieldstrength.y << " uT, ";
    cout << "z = " << tcm2data.fieldstrength.z << " uT" << endl;
    cout << "Temperature: " << tcm2data.temperature << " C" << endl << endl;

    cout << "Limits:" << endl;

    float min, max;
    tcm2->getMinMaxTemperature(min, max);
    cout << "Temperature range: " << min << " .. " << max << " C" << endl;

    tcm2->getMinMaxPitch(min, max);
    cout << "Pitch range: " << rad2Deg(min);
    cout << " .. " << rad2Deg(max) << " deg" << endl;
    tcm2->getMinMaxRoll(min, max);
    cout << "Roll range: " << rad2Deg(min);
    cout << " .. " << rad2Deg(max) << " deg" << endl;

    tcm2->getMinMaxFieldStrength(min, max);
    cout << "Fieldstrength range: " << min << " .. " << max << " uT" << endl;
  }
  catch (const CORBA::Exception & e) {
    cerr << "Uncaught CORBA exception: " << e << endl;
    return 1;
  }
  return 0;
}

