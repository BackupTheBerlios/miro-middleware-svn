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
#ifndef WindowArbiterMessage_h
#define WindowArbiterMessage_h

#include "VelocityC.h"
#include "ArbiterMessage.h"

namespace Miro
{
  class WindowArbiterMessage : public ArbiterMessage
  {
    typedef ArbiterMessage Super;
  public:
    WindowArbiterMessage();

    VelocityIDL velocity;

  protected:
    void printToConsole(std::ostream& ostr) const;
  };
};
#endif
