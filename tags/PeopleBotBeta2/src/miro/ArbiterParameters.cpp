// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ArbiterParameters.h"
#include "Behaviour.h"

namespace Miro
{
  ArbiterParameters::ArbiterParameters() :
    nextNumber_(0UL),
    pattern(NULL)
  {}

  ArbiterParameters::~ArbiterParameters()
  {}

  void
  ArbiterParameters::operator <<= (const QDomNode&)
  {}

  void
  ArbiterParameters::registerBehaviour(Behaviour * _behaviour)
  {
    priorities[_behaviour] = nextNumber_;
    ++nextNumber_;
  }
  void
  ArbiterParameters::printToStream(std::ostream& ostr) const
  {
    RegistrationMap::const_iterator i;
    for (i = priorities.begin(); i != priorities.end(); ++i) {
	ostr << i->first->getBehaviourName() 
	     << "'s priority: " << i->second << endl;
    }
  }

  std::ostream&
  operator << (std::ostream& ostr, const ArbiterParameters& _params)
  {
    _params.printToStream(ostr);
    return ostr;
  }
};
