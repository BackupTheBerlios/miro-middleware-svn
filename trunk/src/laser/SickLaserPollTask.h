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
#ifndef laserPollTask_hh
#define laserPollTask_hh

#include "miro/Log.h"
#include "miro/Thread.h"

namespace Laser
{
  class Parameters;
};

namespace Miro 
{
  // forward declaration
  class LaserConnection;
  class LaserEvent;
  class LaserStatistic;

  class LaserPollTask : public Thread, public Log
  {
  public:
    //! Constructor
    LaserPollTask(LaserConnection& _laser,
		  LaserEvent& _event,
		  LaserStatistic * _laserStatistic);

    //! Destructor
    virtual ~LaserPollTask();

    /**
     * this is the task main loop, which defines the work to be done
     * this is worked upon on the number of threads that are created
     * inside this task
     */
    virtual int svc ();

  protected:
    /**
     * reference to the LaserImpl class, for signaling data
     * via condition variables
     */
    LaserConnection& laser;
    LaserEvent&      event;
    const ::Laser::Parameters& description;
    LaserStatistic * laserStatistic;
  };

};


#endif




