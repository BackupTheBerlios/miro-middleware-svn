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
#ifndef McpEventHandler_h
#define McpEventHandler_h

#include "miro/DevEventHandler.h"

// forward declerations
class ACE_Reactor;
namespace Miro 
{
  class Exception;
  class CException;
};

namespace Mcp
{
  // forward declerations
  class Message;
  class Connection;
  class Consumer;
  class Parameters;

  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;

    // mcp event handling defines
    // states of mcp io packet
    typedef enum { NO_STARTS, ONE_START, IN_PROGRESS, 
		   START_IN_STREAM, END_IN_STREAM } state_type;
    // values indicating beginning and end of a mcp message
    static const char START = 2;
    static const char END = 3;

  public:
    EventHandler(Connection& _mcp, Consumer* _consumer, 
		 const Parameters& _parameters);
    // private:
    virtual ~EventHandler();

    virtual int handle_input (ACE_HANDLE fd); // fd select
    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  private:
    void startUp();
    void watchdogCancel();

    // references to other objects
    Connection& connection;

    unsigned int watchdogTimeout;

    // status variables for processing of
    // data coming asychronously form the mcp
    static const int BUFFER_SIZE = 256;

    state_type state;
    char buffer[BUFFER_SIZE];
    int  buffLen;
    int  buffPos;

    char * writePtr;
  };
};
#endif




