// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "PsosParameters.h"

#include <iostream>

namespace Psos
{
  Parameters::Parameters()
  {
    // none standart tty settings
    ttyParams.baudrate = 9600;
  }

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc) 
  {
    ostr << static_cast<const Parameters::Super&>(desc);

    return ostr;
  }
};
