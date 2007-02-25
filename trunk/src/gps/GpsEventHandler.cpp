// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
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
//
// Authors: 
//   Ingo van Lil
//
#include "GpsEventHandler.h"
#include "GpsMessage.h"

#include "miro/Exception.h"
#include "miro/Log.h"

#include <iostream>
#include <ace/OS.h>

namespace GPS
{
  EventHandler::EventHandler(Miro::DevConsumer * _consumer) :
    Super(_consumer, new Message()),
    state_(WAIT_SOS),
    pos_(0)
  {
    MIRO_LOG_CTOR("GPS::EventHandler");
    MIRO_LOG_CTOR_END("GPS::EventHandler");
  }

  EventHandler::~EventHandler()
  {
    MIRO_LOG_DTOR("GPS::EventHandler");
    MIRO_LOG_DTOR_END("GPS::EventHandler");
  }

  int EventHandler::handle_input(ACE_HANDLE fd)
  {
    int bpos = 0;
    int n = ACE_OS::read(fd, buffer_, BUFSIZE);

    if (n < 0)
      throw Miro::Exception("GPS::EventHandler: Error reading from GPS device");
    else if (n == 0)
      throw Miro::Exception("GPS::EventHandler: No data available");

    while (bpos < n) {
      switch (state_) {
	case WAIT_SOS: // Wait for start of sentence character ('$')
	  if (buffer_[bpos] == '$') {
	    sentence_[pos_++] = buffer_[bpos];
	    state_ = COPY_MSG;
	  }
	  break;

	case COPY_MSG: // Copy message into sentence buffer
	  if (buffer_[bpos] == '\r') {
	    state_ = WAIT_LF;
	  }
	  else if (buffer_[bpos] == '\0') {
	    MIRO_LOG(LL_WARNING, "GPS::EventHandler: NUL character whithin NMEA sentence");
	    pos_ = 0;
	    state_ = WAIT_SOS;
	  }
	  else if (buffer_[bpos] == '$') {
	    MIRO_LOG(LL_WARNING, "GPS::EventHandler: SOS character whithin NMEA sentence");
	    sentence_[0] = '$';
	    pos_ = 1;
	  } 
	  else if (pos_ >= BUFSIZE-1) {
	    MIRO_LOG(LL_WARNING, "GPS::EventHandler: NMEA sentence too long");
	    pos_ = 0;
	    state_ = WAIT_SOS;
	  } 
	  else
	    sentence_[pos_++] = buffer_[bpos];
	  break;

	case WAIT_LF: // Wait for final terminator, handle message if OK
	  if (buffer_[bpos] == '\n') {
	    sentence_[pos_] = '\0';
	    MIRO_LOG(LL_PRATTLE, sentence_);
	    // pass correctly received sentence to consumer
	    GPS::Message * pmsg = dynamic_cast<GPS::Message *>(message_);
	    pmsg->time() = ACE_OS::gettimeofday();
	    pmsg->set(sentence_);
	    if (pmsg->valid())
	      Super::dispatchMessage();
	    else
	      MIRO_LOG(LL_WARNING, "GPS::EventHandler: Invalid NMEA sentence");
	  }
	  pos_ = 0;
	  state_ = WAIT_SOS;
	  break;

	default:
	  throw Miro::Exception("GPS::EventHandler: illegal state");
      }
      bpos++;
    }

    return 0;
  }
}

