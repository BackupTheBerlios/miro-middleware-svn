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
#ifndef PsosParameters_h
#define PsosParameters_h

#include "miro/TtyParameters.h"

namespace Psos
{
  class Parameters;

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);

  class Parameters : public Miro::TtyParameters
  {
    typedef Miro::TtyParameters Super;

  public:
    Parameters();
    
    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };

};

#endif
