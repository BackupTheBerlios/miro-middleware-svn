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
#include "miro/TtyConnection.h"

namespace FaulMotor
{
  class Connection;
};

namespace FaulTty
{
  // forward declarations
  class EventHandler;

  class Connection : public Miro::TtyConnection
  {
    typedef Miro::TtyConnection Super;

  public:
    Connection(ACE_Reactor* _reactor,
	       EventHandler* _eventHandler,
	       const Miro::TtyParameters& _parameters);
    virtual ~Connection();

    void writeMessage(char const * const _message);

  protected:
    EventHandler* eventHandler;

    Miro::Mutex mutex_;
    ACE_Time_Value lastWrite_;

    friend class FaulMotor::Connection;
  };
};
#endif
