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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#include "McpConsumer.h"
#include "McpConnection.h"
#include "McpMessage.h"
#include "McpDevice.h"

#include "miro/Exception.h"

#include <iostream>

#undef DEBUG

/***************************************************************************
*
*  Mcp Async Module 
*
*  Handles output from base
*
***************************************************************************/

namespace Mcp
{
  using std::cout;
  using std::cerr;
  using std::endl;

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
	cout << "undocumented error: 0x" << std::hex << (int)msg.value() << std::dec;
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
	   << std::hex << (int)msg.opcode() << std::dec << endl;
    }
  }
};
