// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "FaulTtyEventHandler.h"
#include "FaulTtyConnection.h"
#include "FaulTtyMessage.h"

#include "miro/Exception.h"

#undef DEBUG

#ifdef DEBUG
#include <iostream>
#include <stdlib.h>

using std::cout;
using std::cerr;
#endif

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace FaulController
{
  EventHandler::EventHandler(Miro::DevConsumer * _consumer, OdometryMessage::Wheel _wheel) :
    Super(_consumer, new OdometryMessage(_wheel)),
    negate_(false)
  {
    DBG(cout << "Constructing FaulTtyEventHandler." << endl);
  }

  EventHandler::~EventHandler()
  {
    DBG(cout << "Destructing FaulTtyEventHandler." << endl);
    if (static_cast<OdometryMessage *>(message_)->wheel_ == 
        OdometryMessage::LEFT)
	          consumer_ = NULL;
  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd)
  {
    DBG(cout << "FaulEventHandler: handle_input" << endl);

    OdometryMessage * msg = static_cast<OdometryMessage *>(message_);

    // read data from the Faul device if we do not have some left
    int bytes = ACE_OS::read(fd, buff_, 511);

    if (bytes == -1)
      throw Miro::CException(errno, "Error on FaulTty file descriptor read.");

    if (bytes == 0)
      throw Miro::Exception("FaulTty file descriptor was called to read 0" \
			    "bytes from the device. I can´t belief this!");

    DBG(buff_[bytes] = 0);
    DBG(cout << "FaulTty message: " << buff_ << endl);

    char const * last = buff_ + bytes;
    for (char const * first = buff_; first != last; ++first) {
      if (msg->ticks_ == 0)
	msg->time() = ACE_OS::gettimeofday();

      if (*first == '-') {
	negate_ = true;
      }
      else if (*first >= '0' && *first <= '9') {
	msg->ticks_ *= 10;
	msg->ticks_ += *first - '0';
      }
      else if (*first == '\r') {
        if (negate_) {
          msg->ticks_ = -msg->ticks_;
          negate_ = false;
        }

	dispatchMessage();
      }
      else if (*first == '\n') {
	msg->ticks_ = 0;
      }
    }
    DBG(cerr << "Read " << bytes << " bytes from FaulTty" << endl);
    DBG(cout << "FaulTtyEventHandler: Done with select" << endl);

    return 0;
  }

};
