// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef BaseEventHandler_h
#define BaseEventHandler_h

#include <ace/Event_Handler.h>

namespace Base
{
  // forward declerations
  class Connection;
  class Consumer;
  class Parameters;

  class EventHandler : public ACE_Event_Handler
  {
  public:
    EventHandler(Connection& _mcp, 
		 Consumer& _consumer, 
		 const Parameters& _parameters);
    virtual ~EventHandler();

    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  private:
    Connection& connection;
    Consumer& consumer;

    const Parameters& parameters; // base parameters
  };
};

#endif




