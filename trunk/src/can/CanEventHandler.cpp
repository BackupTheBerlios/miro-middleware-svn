// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
//
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors:
//   Stefan Enderle,
//   Stefan Sablatnoeg,
//   Hans Utz
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include <ace/Message_Block.h>

#include "CanEventHandler.h"
#include "CanConnection.h"
#include "CanMessage.h"
#include "Parameters.h"
#include "pcan.h"
#include "errno.h"

#include "miro/TimeHelper.h"

#include <iostream>

#undef DEBUG

#ifdef DEBUG
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Can
{
  using std::cout;
  using std::cerr;
  using std::endl;

  int canCounter = 0;

  Message * EventHandler::newMessage(Parameters const * _params){

     Message * msg;
     if(_params->module == "pcan"){

        msg = new Message();

     }
     else{

        msg = new Message();
     }


  }

  EventHandler::EventHandler(Miro::DevConsumer * _consumer, Parameters const * _params) :
    Super(_consumer, newMessage(_params)),
    msg(static_cast<Message *>(message_)),
    params_(_params)
  {
    DBG(cout << "Constructing CanEventHandler." << endl);
  }

  EventHandler::~EventHandler()
  {
    DBG(cout << "Destructing CanEventHandler." << endl);
  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd)
  {

    DBG(cout << "canEventHandler: handle_input" << endl);
    //cout << "CanEventHandler vor ReadMessage" << endl;
    int count;
    if(params_->module == "pcan"){
       pcanmsg * msgp_;
//       std::cout << "msgp " << (void *) msgp_ << endl;
       msg->canMessage((int **) &msgp_);
//       std::cout << "msgp " << (void *) msgp_ << endl;
       count = ioctl(fd, PCAN_READ_MSG, msgp_);
       if (count == 0) {
         cerr << "Pcan handle_input failed!" << endl;
         return 0;
       }



    }
    else {
       canmsg * msg_;
       msg->canMessage((int **) &msg_);
       count = ACE_OS::read(fd, msg_, sizeof(canmsg));
       if (count == 0) {
         cerr << "handle_input called with no data!" << endl;
         return 0;
       }
       if (count != sizeof(canmsg)) {
         cerr << "canEventHandler: read() != sizeof(canmsg): "
	   << count << " != " << sizeof(canmsg) << endl;
         return 0;
       }
    }

    msg->time() = ACE_OS::gettimeofday(); // set time stamp
#ifdef DEBUG
    if ((++canCounter % 100) == 0)
      cout << "Time: " << msg->time() << endl
	   << "Processed Can mesages: " << canCounter << endl;
#endif

    dispatchMessage();

#ifdef DEBUG
    if ((canCounter % 100) == 0) {
      ACE_Time_Value delta = ACE_OS::gettimeofday();
      delta -= msg->time();

      cout << "Time for event handling: " << delta << endl;
    }
#endif

    DBG(cerr << "Done with select" << endl);

    return 0;
  }
};
