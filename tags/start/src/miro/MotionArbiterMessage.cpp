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


#include "MotionArbiterMessage.h"

#include <iostream>

namespace Miro
{
  MotionArbiterMessage::MotionArbiterMessage() :
    Super()
  {
    velocity.translation = 0;
    velocity.rotation = 0;
  }

  void
  MotionArbiterMessage::printToConsole(std::ostream& ostr) const
  {
    Super::printToConsole(ostr);
    ostr << "translation: " << velocity.translation << endl
	 << "rotation: " << velocity.rotation << endl;
  }
};
