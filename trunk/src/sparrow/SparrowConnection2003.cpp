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


#include "SparrowConnection2003.h"
#include "SparrowEventHandler2003.h"
#include "SparrowConsumer2003.h"
#include "SparrowDevice.h"
#include "Parameters.h"



#include "miro/Exception.h"

#include <ace/Reactor.h>

#ifdef DEBUG
#define DBG(x) x
#define CSDBG(x) x
#else
#define DBG(x)
#define CSDBG(x)
#endif

namespace
{
  ACE_Time_Value stallReset(2, 0);
};

namespace Sparrow
{
  using std::cout;
  using std::cerr;
  using std::endl;

  using Can::Message;

  const int Connection2003::LEFT_MOTOR = 0;
  const int Connection2003::RIGHT_MOTOR = 1;

  //ACE_Time_Value maxTimeout(0, 500000);

  //------------------------//
  //----- constructors -----//
  //------------------------//

  Connection2003::Connection2003(ACE_Reactor* _reactor,
			 Miro::DevEventHandler* _devEventHandler,
			 //			 EventHandler _eventHandler,
			 Consumer2003 * _consumer) :
    Super(_reactor, _devEventHandler),
    consumer(_consumer),
    eventHandler(new EventHandler2003(*this)),
    boardReply(-1)
  {
    DBG(cout << "Constructing SparrowConnection2003." << endl);
    std::cout << "_Reactor " << (void*) _reactor << endl;
    std::cout << "Reactor " << (void*)  reactor << endl;
    std::cout << "this->Reactor " << (void*)  this->reactor << endl;

    // Complete initialization after entering of the reactor processing loop.
    // So we start immediately after the start of the reactor
    ACE_Time_Value startReports(0, 1);
    if (reactor->schedule_timer(eventHandler,
				(void *)INIT_TIMER, // timer id
				startReports,       // delay
				ACE_Time_Value(5)) // interval
	== -1)
      throw Miro::ACE_Exception(errno, "Failed to register timer for status report startup.");


    /*DBG(cout << "polling buttons every " << params_->buttonsPollInterval << "sec" << endl);
    if ( (buttonsPollTimerId = reactor->schedule_timer(eventHandler,
				(void *)BUTTONS_TIMER, // timer id
				params_->buttonsPollInterval, // delay
				params_->buttonsPollInterval) ) // interval
	== -1)
      throw Miro::ACE_Exception(errno, "Failed to register timer for buttons polling.");*/
      std::cout << "Ende Constructor SparrowConnectione 2003" << endl;
  }

  void
  Connection2003::init()
  {
     setInfrared1WaitTime(40);
     setInfrared2WaitTime(40);

  }


  //----------------------//
  //----- destructor -----//
  //----------------------//

  Connection2003::~Connection2003()
  {
    DBG(cout << "Destructing SparrowConnection." << endl);


    //reactor->cancel_timer(buttonsPollTimerId);
    delete eventHandler;
  }

  //-------------------//
  //----- methods -----//
  //-------------------//


  //----------- commands ----------- //

  // set absolute power for both wheels    //
  // positive values are forward, negatives backward //

  void
  Connection2003::infraredGet(unsigned short /*msec*/ , unsigned short /*times*/)
  {

  }

  bool
  Connection2003::infraredAlive()
  {
     return true;
  }


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
  Connection2003::setServo(unsigned char servo, double rad)
  {
    CanMessage message;
    message.length(3);
    message.id(CAN_SERVO_GO_2003);
    message.byteData(0, servo);                         // servo number
    message.shortData(1, (servo)? rad2servo1Ticks(rad) : rad2servo0Ticks(rad));
    write(message);
  }

  void
  Connection2003::kick(unsigned char ventilatetime, unsigned char kicktime)
  {
    std::cout << "Hallo Kicker" << endl;
    CanMessage message;
    message.length(3);
    std::cout << "Hallo Kicker2" << endl;
    message.id(CAN_KICK_2003);
    message.byteData(0,0);
    message.byteData(1, ventilatetime);
    message.byteData(2, kicktime);
    std::cout << "Hallo Kicker3 " << endl;
    write(message);
    std::cout << "Hallo Kicker4" << endl;

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


};
