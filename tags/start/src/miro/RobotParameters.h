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
#ifndef RobotParameters_h
#define RobotParameters_h

#include "Parameters.h"

#include <string>

namespace Miro
{
  class RobotParameters : public Parameters
  {
    typedef Parameters Super;
  public:
    std::string name;
    std::string type;
    std::string nameServiceIOR;

    RobotParameters();
    virtual void operator <<= (const QDomNode& node);
  };

  std::ostream&
  operator << (std::ostream& ostr, const RobotParameters& desc);
};

#endif
 
