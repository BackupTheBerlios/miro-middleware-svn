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
DummyArbiterMessage::printToConsole(std::ostream& _ostr) const
{
  Super::printToConsole(_ostr);
  _ostr << "message: " << message << std::endl;
}
