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

#include <ace/Event_Handler.h>

namespace FaulMotor
{
  // forward declarations
  class Connection;

  //! Timed Event for communication.
  /** 
   * We have to poll the odometry values. But we can't poll while we
   * set a motor command. So in order to gain equidistant odometry
   * probes, we write a sequence of an acceleration, a velocity and a
   * odomertry polling command every time.
   */
  class TimerEventHandler : public ACE_Event_Handler
  {
    //! Super class accessor.
    typedef ACE_Event_Handler Super;

  public:
    //! Initializing consructor.
    /** 
     * We need a reference to the connection, in order
     * to send the commands.
     */
    TimerEventHandler(Connection& _connection);
    //! Virtual constructor for cleanup.
    virtual ~TimerEventHandler();

    //! Inherited callback method.
    virtual int handle_timeout (const ACE_Time_Value &current_time,
				const void *act = 0);

  protected:
    //! Reference to the connection to send the commands.
    Connection& connection_;
    //! Counter to track failures of odometry responses.
    /** 
     * Those can happen due to latencies, but ongoing failure
     * indicates a crash of the lowlevel controllers.
     */
    unsigned int stallCounter_;
  };
}

