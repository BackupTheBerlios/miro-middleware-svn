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
  PowerArbiterMessage::printToConsole(std::ostream& ostr) const
  {
    Super::printToConsole(ostr);
    ostr << "left power (+/- 30000): " << left << endl
	 << "right power (+/- 30000): " << right << endl;
  }
};
