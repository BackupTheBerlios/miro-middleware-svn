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
#ifndef MotionArbiterMessage_h
#define MotionArbiterMessage_h

#include "VelocityC.h"
#include "ArbiterMessage.h"

namespace Miro
{
  //! Arbitration message for the MotionArbiter
  class MotionArbiterMessage : public ArbiterMessage
  {
  public:
    //! The requested velocity.
    VelocityIDL velocity;

    //! Default constructor.
    MotionArbiterMessage();
    //! Virtual assignement operator.
    virtual void assign(const ArbiterMessage& _lhs);

  protected:
    //! Dump arbitration message to the specified output stream.
    void printToConsole(std::ostream& _ostr) const;

  private:
    //! Type of this class.
    typedef MotionArbiterMessage Self;
    //! Super class type.
    typedef ArbiterMessage Super;
  };
};
#endif
