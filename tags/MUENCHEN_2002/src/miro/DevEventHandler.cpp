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


#include "miro/DevEventHandler.h"
#include "miro/DevConsumer.h"
#include "miro/DevMessage.h"

#include "miro/Exception.h"

// #undef DEBUG

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

namespace Miro
{
  DevEventHandler::DevEventHandler(DevConsumer * _consumer, 
				   DevMessage * _message) :
    Super(),
    consumer_(_consumer),
    message_(_message)
  {
    DBG(cout << "Constructing DevEvent." << endl);
  }

  DevEventHandler::~DevEventHandler()
  {
    DBG(cout << "Destructing DeviceEvent." << endl);

    delete consumer_;
    delete message_;
  }

  int
  DevEventHandler::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
  {
    DBG(cout << "Device handle_close called." << endl);

    // commit suizide
    delete this;
    return 0;
  }

  void
  DevEventHandler::dispatchMessage() 
  {
    consumer_->handleMessage(message_);
  }
};
