// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "PowerArbiterMessage.h"

namespace Miro
{
  /**
   * Initializing the wheel powers to zero.
   */
  PowerArbiterMessage::PowerArbiterMessage() :
    Super(),
    left(0),
    right(0)
  {
  }

  void
  PowerArbiterMessage::assign(const ArbiterMessage& _lhs)
  {
    Super::assign(_lhs);
    const Self& pam = dynamic_cast<const Self&>(_lhs);
    left = pam.left;
    right = pam.right;
  }

  void
  PowerArbiterMessage::printToConsole(std::ostream& _ostr) const
  {
    Super::printToConsole(_ostr);
    _ostr << "left power (+/- 30000): " << left << std::endl
	  << "right power (+/- 30000): " << right << std::endl;
  }
};
