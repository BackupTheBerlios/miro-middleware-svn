// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "TimerEventHandler.h"
#include "FaulTtyConnection.h"
#include "FaulTtyMessage.h"

#undef DEBUG

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

namespace FaulTty
{
    TimerEventHandler::TimerEventHandler(Connection& _connection) :
    connection(_connection)
  {
    DBG(cout << "Constructing TimerEventHandler." << endl);
  }


  TimerEventHandler::~TimerEventHandler()
  {
    DBG(cout << "Destructing TimerEventHandler." << endl);


  }

  // timer callback
  int
  TimerEventHandler::handle_timeout(const ACE_Time_Value &, const void *arg)
  {
    
    char buffer[10];

    strcpy(buffer,"0pos\r\n\0");
    Message speedMessageL(buffer); // build speed packet
    connection.writeMessage(speedMessageL);             // send it

    strcpy(buffer,"1pos\r\n\0");
    Message speedMessageR(buffer); // build speed packet
    connection.writeMessage(speedMessageR);
    
    return 0;
  }





};
