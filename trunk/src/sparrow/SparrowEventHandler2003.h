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
#ifndef SparrowEventHandler2003_h
#define SparrowEventHandler2003_h

#include <ace/Event_Handler.h>

namespace Sparrow
{
  // forward declerations
  class Connection2003;

  class EventHandler2003 : public ACE_Event_Handler
  {
  public:
    EventHandler2003(Connection2003& _connection);
    virtual ~EventHandler2003();

    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  protected:
    void initCanConnection() const;

    Connection2003& connection;

  public:

  };
};

#endif




