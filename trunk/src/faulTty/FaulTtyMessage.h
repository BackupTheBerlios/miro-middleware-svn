// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
