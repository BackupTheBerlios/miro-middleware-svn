// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
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
    enum Wheel { LEFT, RIGHT, DELTA_LEFT, DELTA_RIGHT };

    OdometryMessage(Wheel _wheel);

    Wheel wheel() const;
    long ticks() const;
    unsigned char clock() const;

    void setTicks(long _ticks);
    void setClock(unsigned char _clock);

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
  OdometryMessage::Wheel
  OdometryMessage::wheel() const {
    return wheel_;
  }
  inline
  long
  OdometryMessage::ticks() const {
    return ticks_;
  }
  inline
  unsigned char
  OdometryMessage::clock() const {
    return clock_;
  }

  inline
  void
  OdometryMessage::setTicks(long _ticks) {
    ticks_ = _ticks;
  }
  inline
  void
  OdometryMessage::setClock(unsigned char _clock) {
    clock_ = _clock;
  }
}
#endif 
