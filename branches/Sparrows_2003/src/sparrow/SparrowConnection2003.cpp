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

#include "can/CanMessage.h"

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
  using Can::Message;

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
    DBG(cout << "Constructing SparrowConnection." << endl);

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
  }

  void
  Connection2003::init()
  {

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
  Connection2003::infraredGet(unsigned short msec, unsigned short times)
  {

  }

  bool
  Connection2003::infraredAlive()
  {
     return true;
  }

};
