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
#include "FaulTtyEventHandler.h"
#include "FaulTtyConnection.h"
#include "FaulTtyMessage.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include <iostream>

namespace FaulController
{
  EventHandler::EventHandler(Miro::DevConsumer * _consumer, OdometryMessage::Wheel _wheel) :
    Super(_consumer, new OdometryMessage(_wheel)),
    index_(0)
  {
    MIRO_LOG_CTOR("FaulController::EventHandler");
  }

  EventHandler::~EventHandler()
  {
    MIRO_LOG_DTOR("FaulController::EventHandler");
    if (static_cast<OdometryMessage *>(message_)->wheel() == 
        OdometryMessage::LEFT)
	          consumer_ = NULL;
  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd)
  {
    MIRO_DBG(FAUL, LL_PRATTLE, "FaulEventHandler: handle_input");

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
      for (int i = 0; i < 5; ++i) {
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

    MIRO_DBG_OSTR(FAUL, LL_PRATTLE, "Read " << bytes << " bytes from FaulTty");
    MIRO_DBG(FAUL, LL_PRATTLE, "FaulController::EventHandler: Done with select");

    return 0;
  }
}
