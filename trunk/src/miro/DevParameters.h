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
#ifndef DevParameters_h
#define DevParameters_h

#include "Parameters.h"

#include <string>

namespace Miro
{
  class DevParameters : public Parameters
  {
    typedef Parameters Super;
  public:
    std::string  device;

    DevParameters();
    virtual ~DevParameters();
    virtual void operator <<= (const QDomNode& node);
  };

  std::ostream&
  operator << (std::ostream& ostr, const DevParameters& desc);
};

#endif
 
