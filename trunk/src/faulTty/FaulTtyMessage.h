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

namespace FaulController
{
  // input message
  class OdometryMessage : public Miro::DevMessage
  {
  public:
    enum Wheel { LEFT, RIGHT };

    OdometryMessage(Wheel _wheel);
    void setTimestamp(ACE_Time_Value const& _timestamp);

    Wheel wheel_;
    long ticks_;
    unsigned char clock_;
  };

  inline
  OdometryMessage::OdometryMessage(Wheel _wheel) :
    wheel_(_wheel),
    ticks_(0),
    clock_(0)
  {}

  inline
  void 
  OdometryMessage::setTimestamp(ACE_Time_Value const& _timestamp) {
     time_ = _timestamp;
  }
}
#endif 
