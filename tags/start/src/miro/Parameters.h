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
#ifndef miro_parameters_hh
#define miro_parameters_hh

#include <iostream>

// forward declarations
class QDomNode;

namespace Miro
{
  class Parameters {
  public:
    virtual ~Parameters() {};
    virtual void operator <<= (const QDomNode& node) = 0;
  };
};

#endif
 
