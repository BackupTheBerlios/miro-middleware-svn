// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
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

    //! Kick
    void kick(unsigned char ventilatetime, unsigned char kicktime);

    void writeWheel(char const * constbuffer, int length, int motor);
    void writeFloodPing(unsigned short fnumber);

    //! 
    void writeHostAlive();
    void writeAccVel(short accL, short accR, short velL, short velR);

    void setInfrared1WaitTime(unsigned char waittime);
    void setInfrared2WaitTime(unsigned char waittime);
    
    void setPanTicksPerDegree(unsigned int ticks);
    unsigned int getPanTicksPerDegree();

    void alivetimeout();

  protected:
    Consumer2003 * consumer;
    EventHandler2003 * eventHandler;
    
    unsigned int panTicksPerDegree_;
    float currentPanPos;			// Pan position as rad

    short rad2servo0Ticks(double rad) const;
    short rad2servo1Ticks(double rad) const;

  public:
    int boardReply; // for watchdog / init

    static int const LEFT_MOTOR = 0;
    static int const RIGHT_MOTOR = 1;
  };

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

