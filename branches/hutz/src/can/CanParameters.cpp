// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "CanParameters.h"

#include <iostream>

using std::ostream;
using std::string;

namespace Can
{
  Parameters::Parameters()
  {
    device = "/dev/can/0";
  }

  ostream&
  operator << (ostream& ostr, const Parameters& desc) 
  {
    ostr << (Parameters::Super&) desc;
    
    return ostr;
  }
};
