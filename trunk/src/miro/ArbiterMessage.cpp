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
  ArbiterMessage::printToConsole(std::ostream& _ostr) const
  {
    _ostr << "id: " << id << std::endl
	  << "active: " << active << std::endl;
  }

  std::ostream&
  operator << (std::ostream& _ostr, const ArbiterMessage& _message)
  {
    _message.printToConsole(_ostr);
    return _ostr;
  }
};
