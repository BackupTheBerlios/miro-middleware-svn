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
#ifndef TtyParameters_h
#define TtyParameters_h

#include "DevParameters.h"

#include <ace/TTY_IO.h>

namespace Miro
{
  // forward declaration
  class TtyParameters;

  std::ostream&
  operator << (std::ostream& ostr, const TtyParameters& desc);

  class TtyParameters : public DevParameters
  {
    typedef DevParameters Super;

  public:
    ACE_TTY_IO::Serial_Params ttyParams;

    TtyParameters();
    virtual ~TtyParameters();
    virtual void operator <<= (const QDomNode& node);

    friend 
    std::ostream& operator << (std::ostream& ostr, const TtyParameters& desc);
  };
};

#endif
