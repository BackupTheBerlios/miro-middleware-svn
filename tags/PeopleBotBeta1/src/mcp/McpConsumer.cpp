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


#include "McpConsumer.h"
#include "McpConnection.h"
#include "McpMessage.h"
#include "McpDevice.h"

#include "miro/Exception.h"

#include <iostream>

#undef DEBUG

using std::cerr;
using std::cout;

/***************************************************************************
*
*  Mcp Async Module 
*
*  Handles output from base
*
***************************************************************************/

namespace Mcp
{
  Consumer::Consumer(Connection& connection_) :
    Super(),
    connection(connection_)
  {
  }

  void
  Consumer::handleWatchdogTimeout()
  {
    throw Miro::Exception("BaseModule: Received watchdog timeout.");
  }

  //
  // protected methods
  //

  void 
  Consumer::handleMessage(const Miro::DevMessage * message)
  {
    const Message& msg = (const Message&)*message;

    switch(msg.opcode()) {
    case 0x00:
      cerr << "mcp communications error: ";
      switch (msg.value()) {
      case 0x01:
	cout << "bad checksum";
	break;
      case 0x02:
	cout << "bad opcode (greadter than 0x7f)";
	break;
      case 0x03:
	cout << "time-out (if record not completed within .5 sec)";
      default:
	cout << "undocumented error: 0x" << hex << (int)msg.value() << dec;
	break;
      }
      cout << " - reseting mcp communication." << endl;

      // send mcp a bunch of 0s to get it in synch
      connection.synch();
      break;
	  
    case 0x01 : // other error
      handleError(msg);
      break;
	  
    case 0x02 :
      break;  /* error clear */ 
	  
    case OP_PSR_DATA:
    case OP_STATUS_REPORT:
      handleStatusReport(msg);
      break;
	  
    case OP_WATCH_DOG:
      handleWatchdogTimeout();
      break;
	  
    case OP_USER_MESSAGE: {
      Miro::Guard guard(connection.comMutex);
      connection.comBlocked = false;
      connection.comCond.signal();
#ifdef DEBUG
      cout << "User message arrived" << endl;
#endif
      break;
    }
    default:
      cerr << __FILE__ << ":" 
	   << __LINE__ << ":" 
	   << __FUNCTION__ << "() - Unhandled opcode 0x" 
	   << hex << (int)msg.opcode() << dec << endl;
    }
  }
};
