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
#ifndef BAP_MotionArbiterMessage_h
#define BAP_MotionArbiterMessage_h

#include "idl/VelocityC.h"
#include "ArbiterMessage.h"

namespace Miro
{
  namespace BAP
  {
    //! Arbitration message for the MotionArbiter
    class MotionArbiterMessage : public ArbiterMessage
    {
      //! Super class type.
      typedef ArbiterMessage Super;
      //! Type of this class.
      typedef MotionArbiterMessage Self;
    public:
      //! The requested velocity.
      VelocityIDL velocity;

      //! Default constructor.
      MotionArbiterMessage();
      //! Virtual assignement operator.
      virtual void assign(const ArbiterMessage& _lhs);

    protected:
      //! Dump arbitration message to the specified output stream.
      void printToStream(std::ostream& _ostr) const;
    };
  }
}
#endif
