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
#include "DummyBehaviourParameters.h"

#include "miro/KeyValueList.h"

#include <iostream>

DummyBehaviourParameters::DummyBehaviourParameters() :
  dummy(0)
{}

void
DummyBehaviourParameters::operator <<= (const Miro::KeyValueList& _params)
{
  Super::operator <<= (_params);

  QString str = _params.getValue("Dummy");
  if (str.length() != 0)
    dummy = str.toInt();
}

void
DummyBehaviourParameters::printToStream(std::ostream& ostr) const
{
  Super::printToStream(ostr);
  ostr << "Dummy: " << dummy << std::endl;
}
