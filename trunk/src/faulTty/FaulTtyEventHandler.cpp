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



#include "FaulTtyEventHandler.h"
#include "FaulTtyConnection.h"
#include "FaulTtyMessage.h"

#include <algorithm>

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

namespace FaulTty
{
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
    DBG(cout << "Constructing FaulTtyEventHandler." << endl);
    lrbytes=0; //init f�r die zwei daten wegen multiplexer
  }

  EventHandler::~EventHandler()
  {
    DBG(cout << "Destructing FaulTtyEventHandler." << endl);

  }

  // file descriptor callback
  int
  EventHandler::handle_input (ACE_HANDLE fd)
  {
    DBG(cout << "FaulEventHandler: handle_input" << endl);

    Message* msg = (Message*) message_;

    int bytes = 0;
    char thisChar;

    // read data from the Faul device if we do not have some left
    bytes = ACE_OS::read(fd, buff, 511);

    if (bytes == -1)
      throw Miro::CException(errno, "Error on FaulTty file descriptor read.");

    if (bytes == 0)
      throw Miro::Exception("FaulTty file descriptor was called to read 0" \
			    "bytes from the device. I can�t belief this!");

    //msg->time() = ACE_OS::gettimeofday();

    if (lrbytes==0) {
        posL=(long)atoi(buff);
	lrbytes=1;
	}
       else {
	posR=(long)atoi(buff);
	lrbytes=0;
	msg->setPos(posL,posR);
	msg->time() = ACE_OS::gettimeofday();
	//cout <<ACE_OS::gettimeofday()<< endl;
       dispatchMessage();
       }

   

//cout << "hab daten gelesen!!!!!!!" << bytes << "   " <<buff<< endl;

    DBG(cerr << "Read " << bytes << " bytes from FaulTty" << endl);


	  

	  dispatchMessage();
	
    DBG(cout << "FaulTtyEventHandler: Done with select" << endl);

    return 0;
  }

  void
  EventHandler::dispatchMessage()
  {
    Message* msg = (Message*) message_;

 
	Super::dispatchMessage();
    
  }

  // timer callback
  int
  EventHandler::handle_timeout(const ACE_Time_Value &, const void *arg) 
  {
    DBG(cout << "FaulTtyEventHandler: handle timer event." << endl);

   
    return 0;
  }


  void
  EventHandler::parseSynchMessage(const Message& _message)
  {
   
  }

  void 
  EventHandler::initDevice()
  {
  
  }
};