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

#include "MotionArbiterMessage.h"

namespace Miro
{
  namespace BAP
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
    MotionArbiterMessage::printToStream(std::ostream& _ostr) const
    {
      Super::printToStream(_ostr);
      _ostr << "translation: " << velocity.translation << std::endl
	    << "rotation: " << velocity.rotation << std::endl;
    }
  }
}
