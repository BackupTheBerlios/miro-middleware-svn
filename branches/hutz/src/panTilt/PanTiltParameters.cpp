// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "PanTiltParameters.h"

namespace DpPanTilt
{
  Miro::Singleton<Parameters> Parameters::instance;

  Parameters::Parameters() 
  {
    // overriding inherited default values

    // device the panTilt is connected to, 
    // usually /dev/panTilt, but maybe any tty
    device = "/dev/panTilt";
    ttyParams.baudrate = 9600;
  }
};
