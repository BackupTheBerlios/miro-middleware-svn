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
#ifndef PsosConnection_h
#define PsosConnection_h

#include "miro/Synch.h"
#include "miro/Exception.h"
#include "miro/TtyConnection.h"

namespace Psos
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
    bool synched() const;
    bool waitSynch(const ACE_Time_Value& maxWait) const;
    void close();

  protected:
    EventHandler* eventHandler;
    int synchTimerId;
    int watchdogTimerId;

    Miro::Mutex writeMutex;
    ACE_Time_Value lastWrite;

    static const ACE_Time_Value writeTimeOut;
  };
}
#endif
