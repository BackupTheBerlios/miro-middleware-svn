// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include <ace/Event_Handler.h>

namespace FaulMotor
{
  // forward declarations
  class Connection;

  class TimerEventHandler : public ACE_Event_Handler
  {
    typedef ACE_Event_Handler Super;

  public:
    TimerEventHandler(Connection& _connection);

    virtual ~TimerEventHandler();


    virtual int handle_timeout (const ACE_Time_Value &current_time,
				const void *act = 0);

  protected:
    Connection& connection_;
    bool firstTime_;
  };
};

