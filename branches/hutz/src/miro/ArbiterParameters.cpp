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


#include "ArbiterParameters.h"
#include "Behaviour.h"

#include <iostream>

namespace Miro
{
  ArbiterParameters::ArbiterParameters()
  {}

  ArbiterParameters::~ArbiterParameters()
  {}

  void
  ArbiterParameters::operator <<= (const QDomNode&)
  {}

  void
  ArbiterParameters::printToStream(std::ostream& ostr) const
  {
    PriorityMap::const_iterator i;
    for (i = priorities.begin(); i != priorities.end(); ++i) {
	ostr << i->first->getBehaviourName() << "'s priority: " << i->second << endl;
    }
  }

  std::ostream&
  operator << (std::ostream& ostr, const ArbiterParameters& _params)
  {
    _params.printToStream(ostr);
    return ostr;
  }
};
