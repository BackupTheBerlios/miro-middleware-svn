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
#ifndef PanTiltConnection_h
#define PanTiltConnection_h

#include "miro/Exception.h"
#include "miro/TtyConnection.h"

namespace DpPanTilt
{
  // forward declarations
  class Message;
  class EventHandler;
  class Parameters;
  class Data;

  class Connection : public Miro::TtyConnection
  {
    typedef Miro::TtyConnection Super;

  public:
    Connection(ACE_Reactor * _reactor, 
	       EventHandler * _eventHandler,
	       Data& _data);
    virtual ~Connection();

    /**
     * tries to send pakets to the panTilt, (status request)
     * if the paket is acknowledged it returns true,
     * does five retries at the moment
     */
    void checkConnection();
    void writeMessage(Message& message);

  protected:
    EventHandler * eventHandler;
    long startupTimerId;
    Data& data_;
  };
};
#endif
