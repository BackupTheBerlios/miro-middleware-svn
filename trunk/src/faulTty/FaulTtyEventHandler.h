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
#ifndef FaulTtyEventHandler_h
#define FaulTtyEventHandler_h

#include "miro/DevEventHandler.h"
#include "miro/Synch.h"

#include <string>

namespace FaulTty
{
  // forward declarations
  class Message;
  class Connection;

  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;

    // FaulTty event handling defines
    // states of FaulTty io packet
    typedef enum { NO_STARTS, START_ONE, GET_LENGTH, IN_PROGRESS } state_type;

  public:
    EventHandler(Miro::DevConsumer* _consumer, Connection& _connection);
    // private:
    virtual ~EventHandler();

    virtual int handle_input (ACE_HANDLE fd); // fd select
    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  protected:
    virtual void dispatchMessage();

    void parseSynchMessage(const Message& _message);
    void initDevice();

    Connection& connection;

  public:
    Miro::Mutex     synchMutex;
    Miro::Condition synchCondition;
    int synch;

    // status variables for processing of
    // data coming asychronously form the mcp
    state_type state;
    //unsigned
    char buff[512];
    int  buffLen;
    int  buffPos;
    int  lrbytes;
    long posL, posR;
    long prePosL, prePosR;
    long posinit;

    unsigned char * writePtr;

    std::string name;
    std::string classname;
    std::string subclass;
  };
};
#endif



