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
#ifndef PowerArbiterMessage_h
#define PowerArbiterMessage_h

#include "ArbiterMessage.h"

namespace Miro
{
  //! Arbitration message for the wheel power arbiter.
  class PowerArbiterMessage : public ArbiterMessage
  {
  public:
    //! The requested left wheel power.
    long left;
    //! The requested right wheel power.
    long right;

    //! Default constructor. 
    PowerArbiterMessage();
    virtual void assign(const ArbiterMessage& _lhs);

  protected:
    //! Dump message to the specified output stream.
    void printToConsole(std::ostream& ostr) const;
  private:
    //! Type of this class.
    typedef PowerArbiterMessage Self;
    //! Super class type.
    typedef ArbiterMessage Super;
  };
};
#endif
