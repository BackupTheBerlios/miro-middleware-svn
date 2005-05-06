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

#include "Angle.h"

#include <iostream>

namespace Miro
{
  // IO friend functions
  std::ostream& 
  operator<<(std::ostream& ostr, Angle alpha)
  {
    return ostr << alpha.angle;
  }

  std::istream& 
  operator>>(std::istream& istr, Angle& alpha)
  {
    double a;
    
    istr >> a;
    alpha = Angle(a);
    return istr;
  }
};

