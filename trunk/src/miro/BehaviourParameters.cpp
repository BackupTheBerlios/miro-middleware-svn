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


#include "BehaviourParameters.h"

#include "KeyValueList.h"

#include <qdom.h>

#include <iostream>

namespace Miro
{
  BehaviourParameters::BehaviourParameters() :
    pattern(NULL)
  {}

  BehaviourParameters::~BehaviourParameters()
  {}

  void
  BehaviourParameters::operator <<= (const KeyValueList&)
  {}

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
