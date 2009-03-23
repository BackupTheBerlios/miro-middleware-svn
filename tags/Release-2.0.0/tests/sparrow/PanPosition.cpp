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
#include "idl/SparrowPanTiltC.h"
#include "miro/Client.h"
#include "miro/Angle.h"
#include "miro/TimeHelper.h"

using Miro::Client;
using Miro::SparrowPanTilt;
using Miro::SparrowPanTilt_var;
using Miro::PanPositionIDL;
using Miro::rad2Deg;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

std::ostream &
operator<<(std::ostream &ostr, const PanPositionIDL &rhs) 
{
  ostr << rad2Deg(rhs.angle) << "° " << rad2Deg(rhs. accuracy) << "°";
  return ostr;
}

int main (int argc, char * argv[])
{
  Client client(argc, argv);
  SparrowPanTilt_var pan = client.resolveName<SparrowPanTilt>("PanTilt");

  while(true) {
    ACE_Time_Value a = ACE_OS::gettimeofday();
    Miro::TimeIDL c;
    Miro::timeA2C(a, c);
    PanPositionIDL position = pan->currentPan(c);
    cout << "pan position: " << position << endl;

    ACE_OS::sleep(ACE_Time_Value(0, 50000));
  }
  return 0;
}
