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

#include "MotionArbiterMessage.h"

namespace Miro
{
  /**
   * Initializing the velocities to zero.
   */
  MotionArbiterMessage::MotionArbiterMessage() :
    Super()
  {
    velocity.translation = 0;
    velocity.rotation = 0.;
  }

  void
  MotionArbiterMessage::assign(const ArbiterMessage& _lhs)
  {
    Super::assign(_lhs);
    velocity = dynamic_cast<const Self&>(_lhs).velocity;
  }

  void
  MotionArbiterMessage::printToConsole(std::ostream& ostr) const
  {
    Super::printToConsole(ostr);
    ostr << "translation: " << velocity.translation << endl
	 << "rotation: " << velocity.rotation << endl;
  }
};
