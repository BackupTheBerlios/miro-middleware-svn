// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/SparrowPanTiltC.h"
#include "miro/Client.h"
#include "miro/Angle.h"
#include "miro/TimeHelper.h"

using Miro::Client;
using Miro::SparrowPanTilt;
using Miro::SparrowPanTilt_var;
using Miro::PanPositionIDL;
using Miro::rad2Deg;

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
