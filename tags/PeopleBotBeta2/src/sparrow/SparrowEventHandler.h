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
#ifndef SparrowEventHandler_h
#define SparrowEventHandler_h

#include <ace/Event_Handler.h>

namespace Sparrow
{
  // forward declerations
  class Connection;

  class EventHandler : public ACE_Event_Handler
  {
  public:
    EventHandler(Connection& _connection);
    virtual ~EventHandler();

    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  protected:
    void initSparrowBoard() const;

    Connection& connection;  

  public:
    int stallTimerId;
  };
};

#endif




