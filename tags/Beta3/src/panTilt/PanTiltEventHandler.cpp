// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "PanTiltEventHandler.h"
#include "PanTiltConnection.h"
#include "PanTiltConsumer.h"
#include "PanTiltMessage.h"

namespace DpPanTilt
{
  using std::cout;
  using std::cerr;
  using std::endl;

  //
  // EventHandler is an object wrapper for the event handler 
  // registered with the reactor
  //
  EventHandler::EventHandler(Connection& _connection, Consumer * _consumer) :
    Super(_consumer, new Message()),
    connection_(_connection),
    msg_(static_cast<Message *>(message_))
  {
    state = WFSTAR;
  }

  //
  // Destructor
  //
  EventHandler::~EventHandler() 
  {	
  }

  //
  // file descriptor callback
  //
  int 
  EventHandler::handle_input (ACE_HANDLE fd) 
  {
    unsigned char thisChar;
  
    // buffer for bytes send form the panTilt    
    unsigned char buff[BUFFSIZE];
    int buffPos = 0;
    int buffLen = 0;

    buffLen = read(fd,buff,BUFFSIZE);

    if (buffLen < -1) {
      throw Miro::Exception("EventHandler::handle_input: Error on  file descriptor");
    }   else if (buffLen == 0) {    
      throw Miro::Exception("EventHandler::handle_input:  fd was called, but 0 bytes for reading");
    } else {
      buffPos = 0;
    } 

    while (buffPos < buffLen) {
      thisChar = buff[ buffPos++ ];

      // cerr << "[" << (unsigned int)(thisChar) << "]" << flush;

      switch(state) {
      case WFSTAR:
	if (thisChar == '*') {
	  state = WFDELIM;
	  msg_->time() = ACE_OS::gettimeofday(); // set time stamp
	}
	else if (thisChar == '!') {
	  state = WFDELIM;
	  msg_->setErrorState();
	}
	else {
	  cerr << "ignoring :|" << thisChar << "|" << endl;
	}
	break;

      case WFDELIM: 
	if (thisChar == '\n') {
	  state = WFSTAR;
	  dispatchMessage();
	  msg_->resetMessage();
	}
	else {
	  //cerr << "using :|" << thisChar << "|" << endl;	  
	  // TODO here append to message
	  msg_->addChar( thisChar );
	}
	break;
      default:
	throw Miro::Exception("EventHandler::handle_input: unknown state in panTiltEvent.");
	break;
      }
    }
    return 0;
  }

  // startup event
  int
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *)
  {
    connection_.checkConnection();
    
    Message p1("ED"); // echo disable
    connection_.writeMessage(p1);
    Message p2("FV"); // verbose feedback
    connection_.writeMessage(p2);

    return 0;
  }
};
