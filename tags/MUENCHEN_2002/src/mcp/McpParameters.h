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
#ifndef McpParameters_h
#define McpParameters_h

#include "miro/TtyParameters.h"

namespace Mcp
{
  class Parameters;

  std::ostream&
  operator << (std::ostream& ostr, const Parameters& desc);

  class Parameters : public Miro::TtyParameters
  {
    typedef Miro::TtyParameters Super;

  public:
    ACE_Time_Value watchdogTimeout;

    Parameters();
    virtual ~Parameters();
    virtual void operator <<= (const QDomNode& node);

    friend
    std::ostream&
    operator << (std::ostream& ostr, const Parameters& desc);
  };

};

#endif
