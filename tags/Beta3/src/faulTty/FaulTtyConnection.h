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
#include "FaulControllerConnection.h"
#include "miro/TtyConnection.h"

namespace FaulMotor
{
  class Connection;
};

namespace FaulController
{
  // forward declarations
  class EventHandler;

  class FaulTtyConnection : public Connection
  {
    typedef Connection Super;

  public:
    FaulTtyConnection(ACE_Reactor* _reactor,
	       EventHandler* _eventHandler,
	       const Miro::TtyParameters& _parameters);
    virtual ~FaulTtyConnection();

    void writeMessage(char const * const _message[]);

  protected:
    EventHandler* eventHandler;

    Miro::TtyConnection * ttyConnection;

    Miro::Mutex mutex_;
    ACE_Time_Value lastWrite_;

    friend class FaulMotor::Connection;
  };
};
#endif
