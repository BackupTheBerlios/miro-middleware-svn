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

#undef DEBUG

#ifdef DEBUG
#include <iostream>

using std::cout;
using std::cerr;
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Can
{
  int canCounter = 0;

  EventHandler::EventHandler(Miro::DevConsumer * _consumer) :
    Super(_consumer, new Message()),
    msg(static_cast<Message *>(message_))
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
    cout << "CanEventHandler vor ReadMessage" << endl;
    int count;
    Parameters * params_ = new Parameters();
    if(params_->module == "pcan"){
       pcanmsg * msgp_;
       std::cout << "msgp " << (void *) msgp_ << endl;
       msg->canMessage(&msgp_);
       std::cout << "msgp " << (void *) msgp_ << endl;
       count = ioctl(fd, PCAN_READ_MSG, msgp_);

    }
    else{
       canmsg * msg_;
       msg->canMessage(&msg_);
       count = ACE_OS::read(fd, msg_, sizeof(canmsg));
    }
    std::cout << "Count " << count << " Errno " << errno << endl;

    // since we are called by the ACE_Reactor, we dont emit exceptions
    // but just return on error. - We could return and deregister the
    // event handler (return 1), but then we also have to shut down the
    // whole server...
    /*if (count == 0) {
      cerr << "handle_input called with no data!" << endl;
      return 0;
    }
    if (count != sizeof(canmsg)) {
      cerr << "canEventHandler: read() != sizeof(canmsg): "
	   << count << " != " << sizeof(canmsg) << endl;
      return 0;
    }*/

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
