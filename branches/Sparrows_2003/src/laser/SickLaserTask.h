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
#ifndef laserTask_hh
#define laserTask_hh

#include "miro/Task.h"
#include "miro/Log.h"

namespace Miro 
{
  // forward declaration
  class LaserConnection;
  class LaserMessage;
  class RangeSensorImpl;
  class LaserStatistic;
  

  /**
   * handles complete packages as assembled by laserEvent
   * gets the packages from its message queue, where 
   * laserEvent puts them.
   * keeps statistics about corrupted packages
   * uses the reference to LaserImpl to signal the data
   * received via condition variables
   */
  class LaserTask : public Task, public Log
  {
    /**
     * main super class
     */
    typedef Task Super;

  public:
    /**
     * Constructor
     */
    LaserTask(LaserConnection& _laser, 
	      RangeSensorImpl& _laserI,
	      LaserStatistic * _laserStatistic = NULL);

    /**
     * Destructor
     */
    virtual ~LaserTask();

    /**
     * this handles one packet that is already decoded from a message
     * block, this used to call this from outside, if we do not run in
     * thread, but for convenience we use it in the threaded approach
     * also
     */
    void doPkt(LaserMessage *data);

    /**
     * this is the task main loop, which defines the work to be done
     * this is worked upon on the number of threads that are created
     * inside this task
     */
    virtual int svc();

  protected:
    /**
     * Laser message interface: the asynchronously called handler
     * does the (hardware specific decoding of the message
     */ 
    virtual void handleMessage(const LaserMessage& message);

  protected:
    /**
     * reference to the LaserImpl class, for signaling data
     * via condition variables
     */
    LaserConnection& laser_;
    RangeSensorImpl& laserI_;

    /**
     * ptr to the central device error statistic
     */
    LaserStatistic * laserStatistic_;
  };
};
#endif




