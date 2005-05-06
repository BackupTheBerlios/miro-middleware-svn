// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef DummyArbiterMessage_h
#define DummyArbiterMessage_h

#include "botsvcs/BAP/ArbiterMessage.h"

#include <string>

//! Arbitration message for the wheel power arbiter.
class DummyArbiterMessage : public Miro::BAP::ArbiterMessage
{
public:
  //! The requested left wheel power.
  std::string message;

  //! Default constructor. 
  DummyArbiterMessage();
  virtual void assign(const ArbiterMessage& _lhs);

protected:
  //! Dump message to the specified output stream.
  virtual void printToStream(std::ostream& ostr) const;
private:
  //! Type of this class.
  typedef DummyArbiterMessage Self;
  //! Super class type.
  typedef ArbiterMessage Super;
};
#endif
