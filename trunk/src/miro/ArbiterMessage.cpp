// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ArbiterMessage.h"

namespace Miro
{
  /**
   * Initializes @ref id to NULL and @ref active to false.
   */
  ArbiterMessage::ArbiterMessage() :
    id(NULL),
    active(false)
  {}

  ArbiterMessage::~ArbiterMessage()
  {
  }

  void
  ArbiterMessage::assign(const ArbiterMessage& _lhs)
  {
    *this = _lhs;
  }

  void
  ArbiterMessage::printToConsole(ostream& ostr) const
  {
    ostr << "id: " << id << endl
	 << "active: " << active << endl;
  }

  std::ostream&
  operator << (ostream& ostr, const ArbiterMessage& _message)
  {
    _message.printToConsole(ostr);
    return ostr;
  }
};
