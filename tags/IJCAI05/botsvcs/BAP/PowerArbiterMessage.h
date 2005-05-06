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
#ifndef BAP_PowerArbiterMessage_h
#define BAP_PowerArbiterMessage_h

#include "ArbiterMessage.h"

namespace Miro
{
  namespace BAP
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
      //! Virtual assignement operator.
      virtual void assign(const ArbiterMessage& _lhs);

    protected:
      //! Dump message to the specified output stream.
      void printToStream(std::ostream& _ostr) const;

    private:
      //! Type of this class.
      typedef PowerArbiterMessage Self;
      //! Super class type.
      typedef ArbiterMessage Super;
    };
  }
}
#endif
