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
#ifndef SparrowConnection2003_h
#define SparrowConnection2003_h

#include "SparrowDevice.h"
#include "sparrow/Parameters.h"

#include "SparrowBaseConnection.h"         // CanConnection, CanMessageClass
#include "can/PCanMessage.h"

//------------------------ SparrowClass ---------------------------//

namespace Sparrow
{
  // forward declaration
  class Consumer2003;
  class EventHandler2003;

  /**
   * Class for using the Sparrow robot
   */
  class Connection2003 : public BaseConnection
  {
    typedef BaseConnection Super;
    typedef Can::PCanMessage CanMessage;

  public:
    Connection2003(ACE_Reactor * _reactor,
	       Miro::DevEventHandler * _eventHandler,
	       Consumer2003 * _consumer);
    virtual ~Connection2003();

    // Inherited method.
    /** Startup event. */
    virtual void init();
    virtual void fini();

    //! Move pan to absolute position.
    void setPan(double _rad);
    //! Move pan to absolute position with defined velocity
    void setPanExt(double _rad,double _vel);
        
    //! Query pan calibration parameters.
    void queryPanTicksPerDegree();
    //! Query pan calibration parameters via timer.
    void deferredQueryPanTicksPerDegree(ACE_Time_Value const& _delay);
    //! Trigger pan recalibration.
    void panReset();
    
    void setPanPosition(float _pos);
    float getPanPosition();

    int getPanTicksPerDegree() const { return params_->pan.ticksPerDegree; }

    //! Kick
    void kick(unsigned char ventilatetime, unsigned char kicktime);

    void writeWheel(char const * constbuffer, int length, int motor);
    void writeFloodPing(unsigned short fnumber);

    //! 
    void writeHostAlive();
    void writeAccVel(short accL, short accR, short velL, short velR);

    void setInfrared1WaitTime(unsigned char waittime);
    void setInfrared2WaitTime(unsigned char waittime);
    
    void alivetimeout();

  protected:
    Consumer2003 * consumer;
    EventHandler2003 * eventHandler;
    
    float currentPanPos_;			// Pan position as rad

    short rad2servo0Ticks(double rad) const;
    short rad2servo1Ticks(double rad) const;

  public:
    int boardReply; // for watchdog / init

    static int const LEFT_MOTOR = 0;
    static int const RIGHT_MOTOR = 1;
  };

  inline
  void
  Connection2003::setPanPosition(float _pos) {
    currentPanPos_ = _pos;
  }

  inline
  float
  Connection2003::getPanPosition() {
    return currentPanPos_ - params_->pan.offset;
  }

  inline
  short
  Connection2003::rad2servo0Ticks(double rad) const
  {
    short pulse = params_->servo0MidPulse;
    if (rad > 0.)
      pulse += static_cast<short>(rint(rad * params_->deg2ServoTicksL * 180. / M_PI));
    else
      pulse += static_cast<short>(rint(rad * params_->deg2ServoTicksR * 180. / M_PI));
    return pulse;
  }

  inline
  short
  Connection2003::rad2servo1Ticks(double rad) const
  {
    return params_->servo1MidPulse +
      static_cast<short>(rint(rad * params_->deg2ServoTicksL * 180. / M_PI));
  }
}

#endif // SparrowConnection2003_h

