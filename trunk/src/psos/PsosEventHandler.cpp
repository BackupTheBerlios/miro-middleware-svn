// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "PsosEventHandler.h"
#include "PsosConnection.h"
#include "PsosMessage.h"
#include "PsosDevice.h"

#include <algorithm>

#undef DEBUG

#include <iostream>

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Psos
{
  using std::cout;
  using std::cerr;
  using std::endl;
  using std::hex;
  using std::dec;

  EventHandler::EventHandler(Miro::DevConsumer * _consumer, Connection& _connection) :
    Super(_consumer, new Message()),
    connection(_connection),
    synchMutex(),
    synchCondition(synchMutex),
    synch(-1),
    state(NO_STARTS),
    buffLen(0),
    buffPos(0),
    writePtr((unsigned char*)message_)
 
  {
    DBG(cout << "Constructing PSOSEventHandler." << endl);
  }

  EventHandler::~EventHandler() 
  {
    DBG(cout << "Destructing PSOSEventHandler." << endl);

    synchMutex.release();
    synchCondition.broadcast();
  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd) 
  {
    DBG(cout << "PSOSEventHandler: handle_input" << endl);

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

    DBG(cerr << "Read " << bytes << " bytes from PSOS" << endl);

    // while there ist data left to parse
    while ((buffPos < buffLen)) {
      thisChar = buff[buffPos];
      DBG(cerr << "Working char " << buffPos 
	  << " (0x" <<  hex << (0xff & (unsigned int)thisChar) << dec << ")" );
    
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
	DBG(cout << " - NO_START" << endl);
	if (thisChar == START_1) {
	  state = START_ONE;

	}
	else {
	  cerr << "Unexpected char " << hex 
	       << (0xff & (unsigned int)thisChar) << dec 
	       << " before start1" << endl;
	}
	break;
      case START_ONE:
	DBG(cout << " - ONE_START" << endl);
	if (thisChar == START_2) {
	  state = GET_LENGTH;

	  msg->header(START_12);
	}
	else {
	  state = NO_STARTS; /* starts only count if in a pair */
	  cerr << "Unexpected char " << hex << (int)thisChar << dec  << " after start1" << endl;
	}
	break;
      case GET_LENGTH:
	DBG(cout << " - GET_LENGTH" << endl);
	state = IN_PROGRESS;

	msg->length(thisChar);
	writePtr = msg->data();

        if (thisChar == 0) {
          DBG(cout << "Discarding zero length message" << endl);
          // reset base output parsing
          state = NO_STARTS;
        }
	break;
      case IN_PROGRESS:
	DBG(cout << " - IN_PROGRESS" << endl);
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
    DBG(cout << "PSOSEventHandler: Done with select" << endl);

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
	  DBG(cout << "PSOS synch state:" << synch << " - got sync0" << endl);
	  if (synch == 0) {
	    synch = 1;
	    connection.writeMessage(MSG_SYNC_1);
	    break;
	  }
	  synch = -1;
	  break; 
	case SF_SYNC_1 : 
	  DBG(cout << "PSOS synch state:" << synch << " - got sync1" << endl);
	  if (synch == 1) {
	    synch = 2;
	    connection.writeMessage(MSG_SYNC_2);
	    break;
	  }
	  synch = -1;
	  break; 
	case SF_SYNC_2 :
	  DBG(cout << "PSOS synch state:" << synch << " - got sync3/connected" << endl);
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
	  DBG(cout << "non synch message within synch mode:" << endl);
	  synch = -1;
	}
      }
    }
    else {
      cerr << "PSOSEventHandler: message with wrong checksum - discarded!" << endl;
    }
  }

  // timer callback
  int
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *arg) 
  {
    DBG(cout << "PSOSEventHandler: handle timer event." << endl);

    Miro::Guard guard(synchMutex);
    switch ((const int)arg) {
    case SYNCH_TIMER:
      DBG(cout << "Synch status: " << synch << endl);
      if (synch < 3) {
        DBG(cout << "Resynching" << endl);
	synch = 0;
	connection.writeMessage(MSG_SYNC_0);    // start synching
      }
      if (false /*synch == 3*/) {
        cout << "cancelling connection" << endl;
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
    DBG(cout << "id="  << _message.id() 
	<< " length=" << _message.length() << endl);
  
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

      cout << "PSOS Synch Name: " << name << endl;
      cout << "PSOS Synch ClassName: " << classname << endl;
      cout << "PSOS Synch Subclass: " << subclass << endl;
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
  }
};
