// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
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
    Connection2003& connection_;
  };
}

#endif // SparrowEventHandler2003_h




