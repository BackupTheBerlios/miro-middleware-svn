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
#ifndef CanParameters_h
#define CanParameters_h

#include "miro/DevParameters.h"

namespace Can
{
  class Parameters;
  
  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);

  class Parameters : public Miro::DevParameters
  {
    typedef Miro::DevParameters Super;

  public:
    Parameters();

    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };
};

#endif
