// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef FaulTtyMessage_h
#define FaulTtyMessage_h

#include "miro/DevMessage.h"

namespace FaulTty
{
  // input message
  class OdometryMessage : public Miro::DevMessage
  {
  public:
    typedef enum { LEFT, RIGHT } Wheel;

    OdometryMessage(Wheel _wheel);

    Wheel wheel_;
    long ticks_;
  }; 

  inline 
  OdometryMessage::OdometryMessage(Wheel _wheel) :
    wheel_(_wheel),
    ticks_(0)
  {}
};
#endif 