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
#include "miro/Log.h"

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
  using std::cout;
  using std::cerr;
  using std::endl;

  EventHandler::EventHandler(Miro::DevConsumer * _consumer, OdometryMessage::Wheel _wheel) :
    Super(_consumer, new OdometryMessage(_wheel)),
    index_(0)
  {
    DBG(cout << "Constructing FaulTtyEventHandler." << endl);
  }

  EventHandler::~EventHandler()
  {
    DBG(cout << "Destructing FaulTtyEventHandler." << endl);
    if (static_cast<OdometryMessage *>(message_)->wheel() == 
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
    int bytes = ACE_OS::read(fd, buff_ + index_, 5 - index_);

    if (bytes == -1)
      throw Miro::CException(errno, "Error on FaulTty file descriptor read.");

    if (bytes == 0)
      throw Miro::Exception("FaulTty file descriptor was called to read 0" \
			    "bytes from the device. I can't belief this!");

    index_ += bytes;
    if (index_ != 5)
      return 0;

    index_ = 0;


    if (Miro::Log::level() >= Miro::Log::LL_PRATTLE && 
	Miro::Log::enabled(Miro::Log::FAUL)) {

      std::ostringstream s;
      std::string wheel = (msg->wheel() == OdometryMessage::LEFT)? "left  " : "right ";
      s << wheel << "length: " << bytes << std::endl << std::hex << wheel;
      for (int i = 0; i < bytes; ++i) {
	s.width(2);
	s.fill('0');
	s << static_cast<unsigned int>((unsigned char)buff_[i]) << " ";
      }
      s << std::dec;
      MIRO_DBG(FAUL, LL_PRATTLE, s.str().c_str());
    }
    
    msg->time() = ACE_OS::gettimeofday();
    msg->setTicks(*(reinterpret_cast<long int *>(&buff_[0])));
    msg->setClock(*(reinterpret_cast<unsigned char *>(&buff_[4])));

    dispatchMessage();

#ifdef FAUL_TTY_USES_ASCII_MODE
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
//	cout << msg << endl;  //debug
      }
      else if (*first == '\n') {
	msg->ticks_ = 0;
      }
    }
#endif
    DBG(cerr << "Read " << bytes << " bytes from FaulTty" << endl);
    DBG(cout << "FaulTtyEventHandler: Done with select" << endl);

    return 0;
  }

  int
  EventHandler::handle_close(ACE_HANDLE fd, ACE_Reactor_Mask _m) 
  {
    // make sure all remaining data arrieved...
    ACE_OS::sleep(ACE_Time_Value(0, 100000));


    char buffer[256];
    int rc = ACE_OS::read(fd, buffer, 256);
    if (rc == -1)
      std::cerr << "FaulTtyEventHandler::handle_close() cleanup read failed: "
		<< errno << std::endl;
    else
      std::cerr << "FaulTtyEventHandler::handle_close() read: " 
		<< rc << std::endl;

    return Super::handle_close(fd, _m);
  }
}
