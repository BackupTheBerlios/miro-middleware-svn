// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef SparrowConnection2003_h
#define SparrowConnection2003_h

#include "SparrowDevice2003.h"
#include "Parameters.h"

#include "SparrowBaseConnection.h"         // CanConnection, CanMessageClass

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

  public:
    Connection2003(ACE_Reactor * _reactor,
	       Miro::DevEventHandler * _eventHandler,
	       Consumer2003 * _consumer);
    virtual ~Connection2003();

    void init();

    void infraredGet(unsigned short msec, unsigned short times);
    bool infraredAlive();

    void setServo(unsigned char servo, double rad);
    void kick(unsigned char ventilatetime, unsigned char kicktime);

    void writeWheel(const char * buffer, int length, int motor);
    

    void setInfrared1WaitTime(unsigned char waittime);
    void setInfrared2WaitTime(unsigned char waittime);


    void alivetimeout();


  protected:

    Consumer2003 * consumer;
    EventHandler2003 * eventHandler;

    short rad2servo0Ticks(double rad) const;
    short rad2servo1Ticks(double rad) const;

  public:
    int boardReply; // for watchdog / init

    static const int LEFT_MOTOR;
    static const int RIGHT_MOTOR;


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
};

#endif

