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
#ifndef SphinxSpeechTask_hh
#define SphinxSpeechTask_hh

#include "miro/Task.h"
#include "miro/Log.h"

namespace Miro 
{

  /**
   * handles complete packages as assembled by laserEvent
   * gets the packages from its message queue, where 
   * laserEvent puts them.
   * keeps statistics about corrupted packages
   * uses the reference to LaserImpl to signal the data
   * received via condition variables
   */
  class SphinxSpeechTask : public Task, public Log
  {
    /**
     * main super class
     */
    typedef Task Super;

  public:
    /**
     * Constructor
     */
    SphinxSpeechTask();

    /**
     * Destructor
     */
    virtual ~SphinxSpeechTask();

    /**
     * this handles one packet that is already decoded from a message
     * block, this used to call this from outside, if we do not run in
     * thread, but for convenience we use it in the threaded approach
     * also
     */
    //    void doPkt(LaserMessage *data);

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
    //    virtual void handleMessage(const LaserMessage& message);

  protected:
  };
};
#endif




