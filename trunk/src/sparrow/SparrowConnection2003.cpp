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

#include "SparrowConnection2003.h"
#include "SparrowEventHandler2003.h"
#include "SparrowConsumer2003.h"
#include "SparrowDevice.h"
#include "Parameters.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include <ace/Reactor.h>

namespace Sparrow
{
  using Can::Message;

  const int Connection2003::LEFT_MOTOR;
  const int Connection2003::RIGHT_MOTOR;

  //------------------------//
  //----- constructors -----//
  //------------------------//

  Connection2003::Connection2003(ACE_Reactor* _reactor,
				 Miro::DevEventHandler* _devEventHandler,
				 Consumer2003 * _consumer) :
    Super(_reactor, _devEventHandler),
    consumer(_consumer),
    eventHandler(new EventHandler2003(*this)),
    boardReply(-1)
  {
    MIRO_LOG_CTOR("SparrowConnection2003");

    // Complete initialization after entering of the reactor processing loop.
    // So we start immediately after the start of the reactor
    ACE_Time_Value startReports(0, 1);
    if (reactor_->schedule_timer(eventHandler,
				&INIT_TIMER, // timer id
				startReports,       // delay
				ACE_Time_Value::zero) // interval
	== -1)
      throw Miro::ACE_Exception(errno, 
				"Failed to register timer for status report startup.");

    deferredQueryPanTicksPerDegree(ACE_Time_Value(0, 10000));
  }

  //----------------------//
  //----- destructor -----//
  //----------------------//
  Connection2003::~Connection2003()
  {
    MIRO_LOG_DTOR("SparrowConnection2003");

    delete eventHandler;
  }

  void
  Connection2003::init()
  {
    setInfrared1WaitTime(params_->infraredPulse.msec());
    setInfrared2WaitTime(params_->infraredPulse.msec());
  }

  void
  Connection2003::fini()
  {
    setInfrared1WaitTime(0);
    setInfrared2WaitTime(0);
  }

  //-------------------//
  //----- methods -----//
  //-------------------//


  //----------- commands ----------- //
  void
  Connection2003::writeWheel(char const * buffer, int length, int motor)
  {
    assert(length <= 8);

    CanMessage message;
    message.length(length);
    if(motor == LEFT_MOTOR)
       message.id(CAN_WRITE_MOTOR_LEFT_2003);
    if(motor == RIGHT_MOTOR)
       message.id(CAN_WRITE_MOTOR_RIGHT_2003);
    message.setBuffer(0, buffer, length);
    write(message);
  }

  void
  Connection2003::writeAccVel(short accL, short accR, short velL, short velR)
  {
    CanMessage message;
    message.length(8);
    message.id(CAN_MOTOR_ACC_VEL2003);
    message.shortData(0, ACE_NTOHS(accL));
    message.shortData(2, ACE_NTOHS(accR));
    message.shortData(4, ACE_NTOHS(velL));
    message.shortData(6, ACE_NTOHS(velR));
    write(message);
  }

  void
  Connection2003::setPan(double _rad)
  {
    CanMessage message;
    message.length(5);
    message.id(CAN_PAN_GO_2005);
    message.byteData(0, 0); // servo number
    message.longData(1, (long)((double)getPanTicksPerDegree()*Miro::rad2Deg(-_rad)) );
    write(message);
  }

  void
  Connection2003::queryPanTicksPerDegree()
  {
    CanMessage message;
    message.length(0);
    message.id(CAN_PAN_TICKSPERDEG_2005);
    write(message);
  }

  void
  Connection2003::deferredQueryPanTicksPerDegree(ACE_Time_Value const& _delay)
  {
    if (reactor_->schedule_timer(eventHandler,
				&PAN_CALIBRATION_TIMER, // timer id
				_delay,       // delay
				ACE_Time_Value::zero) // interval
	== -1)
      throw Miro::ACE_Exception(errno, 
				"Failed to register timer for Pan2005 calibration.");
  }

  void
  Connection2003::panReset()
  {
    CanMessage message;
    message.length(0);
    message.id(CAN_PAN_RESET_2005);
    write(message);
  }

  void
  Connection2003::kick(unsigned char ventilatetime, unsigned char kicktime)
  {
    CanMessage message;
    message.length(3);
    message.id(CAN_KICK_2003);
    message.byteData(0,0);
    message.byteData(1, ventilatetime);
    message.byteData(2, kicktime);
    write(message);
  }

  void
  Connection2003::alivetimeout()
  {
    CanMessage message;
    message.length(1);
    message.id(CAN_READ_PORTS);
    message.byteData(0,0);
    write(message);
  }

  void
  Connection2003::setInfrared1WaitTime(unsigned char waittime)
  {
    CanMessage message;
    message.length(2);
    message.id(CAN_IR_SET_FREQ1_2003);
    message.byteData(0, 1);
    message.byteData(1, waittime);
    write(message);
  }

  void
  Connection2003::setInfrared2WaitTime(unsigned char waittime)
  {
    CanMessage message;
    message.length(2);
    message.id(CAN_IR_SET_FREQ2_2003);
    message.byteData(0, 1);
    message.byteData(1, waittime);
    write(message);
  }

  void 
  Connection2003::setPanTicksPerDegree(unsigned int ticks)
  {
    panTicksPerDegree = ticks;
  }

  unsigned int 
  Connection2003::getPanTicksPerDegree()
  {
    return(panTicksPerDegree);
  }


  void
  Connection2003::writeFloodPing(unsigned short fnumber)
  {

    CanMessage message;
    message.length(2);
    message.id(CAN_MOTOR_FLOOD_2003);
    message.shortData(0, fnumber);
    write(message);
  }


  void
  Connection2003::writeHostAlive()
  {
    CanMessage message;
    message.length(1);
    message.id(CAN_HOST_ALIVE_2003);
    message.byteData(0,0);
    write(message);
  }
}
