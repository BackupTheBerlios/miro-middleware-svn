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
#include "PsosEventHandler.h"
#include "PsosConnection.h"
#include "PsosMessage.h"
#include "PsosDevice.h"
#include "Parameters.h"

#include "miro/Log.h"

#include <algorithm>

namespace Psos
{
  EventHandler::EventHandler(Miro::DevConsumer * _consumer, 
			     Connection& _connection,
			     Parameters const * _params) :
    Super(_consumer, new Message()),
    connection(_connection),
    params_(_params),
    synchMutex(),
    synchCondition(synchMutex),
    synch(-1),
    state(NO_STARTS),
    buffLen(0),
    buffPos(0),
    writePtr((unsigned char*)message_)
 
  {
    MIRO_LOG_CTOR("Psos::EventHandler");
  }

  EventHandler::~EventHandler() 
  {
    MIRO_LOG_DTOR("Psos::EventHandler");

    synchMutex.release();
    synchCondition.broadcast();
  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd) 
  {
    MIRO_DBG(PIONEER, LL_PRATTLE, "Psos::EventHandler: handle_input");

    Message* msg = (Message*) message_;

    int bytes = 0;
    char thisChar;

    // read data from the PSOS device if we do not have some left
    bytes = ACE_OS::read(fd, buff, 511);

    if (bytes == -1)
      throw Miro::CException(errno, "Error on PSOS file descriptor read.");

    if (bytes == 0)
      throw Miro::Exception("PSOS file descriptor was called to read 0" \
			    "bytes from the device. I can,A4(Bt belief this!");

    buffLen = bytes;
    buffPos = 0;
    int len, n;

    MIRO_DBG_OSTR(PIONEER, LL_PRATTLE, "Read " << bytes << " bytes from PSOS");

    // while there ist data left to parse
    while ((buffPos < buffLen)) {
      thisChar = buff[buffPos];
      MIRO_DBG_OSTR(PIONEER, LL_PRATTLE,
		    "Working char " << buffPos <<
		    " (0x" <<  std::hex << (0xff & (unsigned int)thisChar) << 
		    std::dec << ")" );
    
      // translate the PSOS message read from fd
      // into an PSOS message string:

      // each message starts with START1 START2 (0xfafb)
      // followed by the length (one byte)

      // message parsing works as follows:
      // identify the beginning of a PSOS message,
      // copy length bytes into a PSOS::Message buffer,
      // hand the PSOS::Message on to further processing to another Consumer

      switch(state) {
      case NO_STARTS:
	MIRO_DBG(PIONEER, LL_DEBUG, " - NO_START");
	if (thisChar == START_1) {
	  state = START_ONE;

	}
	else {
	  MIRO_LOG_OSTR(LL_ERROR, 
			"Psos::EventHandler: Unexpected char " << std::hex <<
			(0xff & (unsigned int)thisChar) << std::dec <<
			" before start1");
	}
	break;
      case START_ONE:
	MIRO_DBG(PIONEER, LL_DEBUG, " - ONE_START");
	if (thisChar == START_2) {
	  state = GET_LENGTH;

	  msg->header(START_12);
	}
	else {
	  state = NO_STARTS; /* starts only count if in a pair */
	   MIRO_LOG_OSTR(LL_ERROR, 
			 "Psos::EventHandler: Unexpected char " << std::hex << 
			 (int)thisChar << std::dec  << 
			 " after start1");
	}
	break;
      case GET_LENGTH:
	MIRO_DBG(PIONEER, LL_PRATTLE, " - GET_LENGTH");
	state = IN_PROGRESS;

	msg->length(thisChar);
	writePtr = msg->data();

        if (thisChar == 0) {
          MIRO_LOG(LL_NOTICE, "Psos::EventHandler: Discarding zero length message.");
          // reset base output parsing
          state = NO_STARTS;
        }
	break;
      case IN_PROGRESS:
	MIRO_DBG(PIONEER, LL_DEBUG, " - IN_PROGRESS");
	len = msg->length() - (writePtr - msg->data());
	n = std::min(len, buffLen - buffPos);
	std::copy(&buff[buffPos], &buff[buffPos] + n, writePtr);
	writePtr += n;
	buffPos += n-1;

	if (len - n == 0) {
	  msg->time() = ACE_OS::gettimeofday();
	  dispatchMessage();
	  // reset base output parsing
	  state = NO_STARTS;
	}
      }
      ++buffPos;
    }
    MIRO_DBG(PIONEER, LL_PRATTLE,"Psos::EventHandler: Done with select.");

    return 0;
  }

  void
  EventHandler::dispatchMessage()
  {
    Message* msg = (Message*) message_;
    
    if (msg->validChecksum()) {
      if (synch >= 3) {
	Super::dispatchMessage();
      }
      else {
	// handle synch messages
        Miro::Guard guard(synchMutex);

	switch (msg->id()) {
	case SF_SYNC_0 : 
	  MIRO_DBG_OSTR(PIONEER, LL_DEBUG,
			"PSOS synch state:" << synch << " - got sync0.");
	  if (synch == 0) {
	    synch = 1;
	    connection.writeMessage(MSG_SYNC_1);
	    break;
	  }
	  synch = -1;
	  break; 
	case SF_SYNC_1 : 
	  MIRO_DBG_OSTR(PIONEER, LL_DEBUG,
			"PSOS synch state:" << synch << " - got sync1.");
	  if (synch == 1) {
	    synch = 2;
	    connection.writeMessage(MSG_SYNC_2);
	    break;
	  }
	  synch = -1;
	  break; 
	case SF_SYNC_2 :
	  MIRO_DBG_OSTR(PIONEER, LL_DEBUG,
			"PSOS synch state:" << synch << " - got sync3/connected.");
	  if (synch == 2) {
            parseSynchMessage(*msg);
	    synch = 3;
	    initDevice();
	    synchCondition.broadcast();
	    break;
	  }
	  synch = -1;
	  break; 
	default:
	  MIRO_LOG(LL_ERROR, "Psos::EventHandler: Non synch message within synch mode.");
	  synch = -1;
	}
      }
    }
    else {
      MIRO_LOG(LL_ERROR, "Psos::EventHandler: message with wrong checksum - discarded!");
    }
  }

  // timer callback
  int
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *arg) 
  {
    MIRO_DBG(PIONEER, LL_PRATTLE,"Psos::EventHandler: handle timer event.");

    Miro::Guard guard(synchMutex);
    unsigned int task = *reinterpret_cast<unsigned int const *>(arg);
    switch (task) {


    case SYNCH_TIMER:
      MIRO_DBG_OSTR(PIONEER, LL_DEBUG, "Synch status: " << synch);
      if (synch < 3) {
        MIRO_DBG(PIONEER, LL_DEBUG, "Resynching.");
	synch = 0;
	connection.writeMessage(MSG_SYNC_0);    // start synching
      }
      if (false /*synch == 3*/) {
        MIRO_DBG(PIONEER, LL_DEBUG, "Cancelling connection.");
        synch = 4;
        connection.writeMessage(MSG_COMCLOSE);
      }
      break;
    case PING_TIMER:
      if (synch == 3)
	connection.writeMessage(MSG_COMPULSE);  // send pulse packet
      break;
    default: 
      throw Miro::Exception("PSOSEventHandler: Unknown timeout type.");
    }
    return 0;
  }


  void
  EventHandler::parseSynchMessage(const Message& _message)
  {
     MIRO_DBG_OSTR(PIONEER, LL_PRATTLE, 
		   "Psos::parseSynchMessage: id="  << 
		   _message.id() << " length=" << _message.length());
  
    char tmp[128];
    int len = 0;

    if (_message.id() == 2) {
      strncpy(tmp, (char *) _message.data(), 128);
      name = tmp;
      len = name.length() + 1;
      strncpy(tmp, (char *)&_message.data()[len], 128 );
      classname = tmp;
      len += classname.length() + 1;
      strncpy(tmp, (char *)&_message.data()[len], 128);
      subclass = tmp;

      MIRO_LOG_OSTR(LL_NOTICE,
		    "PSOS Synch Name: " << name << std::endl <<
		    "PSOS Synch ClassName: " << classname << std::endl <<
		    "PSOS Synch Subclass: " << subclass);
    }
  }

  void 
  EventHandler::initDevice()
  {
    // open connection,
    // start delivery of standard SIPs
    connection.writeMessage(MSG_COMOPEN);
	    
    // if it's the Ppb request IOpac SIP
    if (true || classname == "ppb") {
      const Message IO_SIP(SF_COMIOREQ, (short)2);
      connection.writeMessage(IO_SIP);
    }

    if (params_->enableMotors) {
      const Message MOTOR_ENABLE(SF_COMENABLE, (short)1);
      connection.writeMessage(MOTOR_ENABLE);
    }
  }
}
