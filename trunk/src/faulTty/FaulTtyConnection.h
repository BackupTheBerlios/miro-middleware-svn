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
#ifndef FaulTtyConnection_h
#define FaulTtyConnection_h

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/TtyConnection.h"

namespace FaulTty
{
  // forward declarations
  class Message;
  class EventHandler;
  class Parameters;

  class Connection : public Miro::TtyConnection
  {
    typedef Miro::TtyConnection Super;

  public:
    Connection(ACE_Reactor* _reactor,
	       EventHandler* _eventHandler,
	       const Parameters& _parameters);
    virtual ~Connection();

    void writeMessage(const Message& message);
    
    void close();

  protected:
    EventHandler* eventHandler;
    int synchTimerId;
    int watchdogTimerId;

    Miro::Mutex writeMutex;
    ACE_Time_Value lastWrite;

    static const ACE_Time_Value writeTimeOut;
  };
};
#endif
