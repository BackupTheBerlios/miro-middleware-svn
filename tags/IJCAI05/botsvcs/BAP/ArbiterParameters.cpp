// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ArbiterParameters.h"
#include "Behaviour.h"

#include <qdom.h>

namespace Miro
{
  namespace BAP
  {
    std::ostream&
    operator << (std::ostream& ostr, const ArbiterParameters& _params)
    {
      _params.printToStream(ostr);
      return ostr;
    }

    ArbiterParameters::ArbiterParameters() :
      nextNumber_(0UL),
      pattern(NULL)
    {}
    
    ArbiterParameters::ArbiterParameters(ArbiterParameters const&) :
      nextNumber_(0UL),
      pattern(NULL)
    {}
    
    /** Noop implementation. */
    ArbiterParameters::~ArbiterParameters()
    {}
    
    /** Noop implementation. */
    void
    ArbiterParameters::operator <<= (const QDomNode&)
    {}

    QDomElement
    ArbiterParameters::operator >>= (QDomNode& _node) const
    {
      QDomDocument d = _node.ownerDocument();
      QDomElement e = d.createElement("arbiter");
      //      e.setAttribute("name", getName());

      _node.appendChild(e);

      QDomElement g;


      return e;
    }

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
	ostr << i->first->getName() 
	     << "'s priority: " << i->second << std::endl;
      }
    }
  }
}
