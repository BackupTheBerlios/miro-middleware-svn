// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "DummyArbiterMessage.h"

DummyArbiterMessage::DummyArbiterMessage() :
  Super(),
  message()
{
}

void
DummyArbiterMessage::assign(const ArbiterMessage& _lhs)
{
  Super::assign(_lhs);
  message = dynamic_cast<const Self&>(_lhs).message;
}

void
DummyArbiterMessage::printToConsole(std::ostream& ostr) const
{
  Super::printToConsole(ostr);
  ostr << "message: " << message << endl;
}
