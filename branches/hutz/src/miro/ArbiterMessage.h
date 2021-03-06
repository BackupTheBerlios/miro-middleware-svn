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
#ifndef ArbiterMessage_h
#define ArbiterMessage_h

#include "MotionC.h"

#include <iostream>

namespace Miro
{
  // forward declarations
  class Behaviour;

  struct ArbiterMessage
  {
    Behaviour * id;
    bool active;

    ArbiterMessage();
    virtual ~ArbiterMessage();
    
    VelocityIDL velocity;

  protected:
    virtual void printToConsole(std::ostream& ostr) const;

    friend 
    std::ostream& operator << (std::ostream& ostr, const ArbiterMessage& _message);

  };

  std::ostream&
  operator << (std::ostream& ostr, const ArbiterMessage& _message);
};
#endif
