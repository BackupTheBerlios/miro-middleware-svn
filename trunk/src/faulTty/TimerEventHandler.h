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

#include <ace/Event_Handler.h>

#include <string>

namespace FaulTty
{
  // forward declarations

  class Connection;

class TimerEventHandler : public ACE_Event_Handler
  {
    typedef ACE_Event_Handler Super;

  public:
    /**
     * Constructor
     *
     * @param consumer_
     * A pointer to the consumer who's message queue is to be filled
     * by the event instance.
     *
     * @param messageSize
     * The size (sizeof()) of the message to be passed. The Event
     * provides a ACE_Message_Block with messageSize bytes in its
     * message buffer to copy the message into. - Well, not type safe,
     * but the alternative would have been to templatize
     * @ref Connection, @ref Event and @ref DevConsumer.
     */

    TimerEventHandler(Connection& _connection);

    virtual ~TimerEventHandler();


    virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);

	Connection& connection;
   };
};

