// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef WindowArbiterParameters_h
#define WindowArbiterParameters_h

#include "ArbiterParameters.h"

// forward declarations
namespace Miro
{
  class Behaviour;
};

namespace Miro
{
  struct WindowArbiterParameters : public ArbiterParameters
  {
    typedef ArbiterParameters Super;

    // void operator <<= (const QDomNode& _node);

  protected:
    // void printToStream(std::ostream& ostr) const;
  };
};
#endif
