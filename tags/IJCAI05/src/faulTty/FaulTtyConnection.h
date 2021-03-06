// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef FaulTtyConnection_h
#define FaulTtyConnection_h

#include "FaulControllerConnection.h"

#include "miro/Synch.h"
#include "miro/TtyConnection.h"

namespace FaulMotor {
  class Connection;
}

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

    virtual void sendAccVelTicks(short _accL, short _accR, 
				 short _velL, short _velR);
    virtual void sendAccVelTicks(short _acc, short _vel);
    virtual void writeBinary(char const * _message, int _len);
    virtual void writeMessage(char const * const _message[]);

  protected:
    Miro::TtyConnection ttyConnection_;

    Miro::Mutex mutex_;
    ACE_Time_Value lastWrite_;

    friend class FaulMotor::Connection;
  };
}
#endif
