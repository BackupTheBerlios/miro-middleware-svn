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


#include "BehaviourParameters.h"

#include <iostream>

namespace Miro
{
  BehaviourParameters::BehaviourParameters() :
    pattern(NULL)
  {}

  BehaviourParameters::~BehaviourParameters()
  {}

  void
  BehaviourParameters::operator <<= (const QDomNode&)
  {}

  QDomElement
  BehaviourParameters::operator >>= (QDomNode& _node) const
  {
    QDomDocument d = _node.ownerDocument();
    QDomElement e = d.createElement("config_item");
    _node.appendChild(e);

    return e;
  }

  void
  BehaviourParameters::printToStream(std::ostream&) const
  {
  }

  std::ostream&
  operator << (ostream& ostr, const BehaviourParameters& _params)
  {
    _params.printToStream(ostr);

    return ostr;
  }
};
