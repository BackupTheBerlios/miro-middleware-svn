// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "TimeSeries.h"
#include "TimeHelper.h"

#include <iostream>

namespace Miro
{
  std::ostream& operator<<(std::ostream& _ostr, TimeStats const& _rhs)
  {
    _ostr << "min: " << _rhs.min << std::endl
	  << "max: " << _rhs.max << std::endl
	  << "mean: " << _rhs.mean << std::endl;

    return _ostr;
  }
}
