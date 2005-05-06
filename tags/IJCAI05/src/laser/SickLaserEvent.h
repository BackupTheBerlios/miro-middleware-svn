// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef laserEvent_hh
#define laserEvent_hh

#include <ace/Event_Handler.h>
#include <ace/Synch.h>
#include <ace/Message_Block.h>
#include <cmath>
#include <ace/OS.h>

#include "SickLaserTask.h"
#include "SickLaserStatistic.h"


namespace Miro {

  // forward declaration
  class LaserTask;

  /**
   * LaserEvent is an object wrapper for the event handler 
   * registered with the reactor.
   *
   * it is provided with a log facility and inherits from 
   * E_Event_Handler
   */
  class LaserEvent : public ACE_Event_Handler
  {
  public:
    /**
     * constructor does initialization
     */
    LaserEvent(LaserTask * _laserTask, LaserStatistic * _laserStatistic);

    /**
     * destructor, only needed because virtual functions are used
     */  
    virtual ~LaserEvent();

  public:
    /**
     * handles file descriptor events
     */
    virtual int handle_input (ACE_HANDLE fd); // fd select

    /**
     * handles close events
     * Called when object is removed from the <ACE_Reactor>.
     */
    virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

    /**
     * timeout is called by laserPollTask when the packet 
     * could not be completed on time, as sending is blocked 
     * until a complete packet is arrived
     */
    void timeout();

    /**
     * sync Mutex max be used to avoid sending to the laser while
     * we receive from it, but this proved to be not pratical for
     * deadlock reasons, and problems while resynchronizing to the
     * laser packet stream.
     */
    ACE_Thread_Mutex                       syncMutex;

    /**
     * for handling the acknowledges of the laser (and making them known
     * in upper layers) we use a mutex/conditionvariable pair and bool
     * when an ackownledge is received ackNAck is set and ackNackCond 
     * is broadcasted.
     */
    ACE_Thread_Mutex                       ackNackMutex;
    ACE_Thread_Condition<ACE_Thread_Mutex> ackNackCond;
    bool                                   ackNack;

    /**
     * state mutex is needed for exclusive access to the state machine
     * we use for decoding packets
     */
    ACE_Thread_Mutex                       stateMutex;

  private:

    /**
     * we need a reference to the Task that handles our packets
     * we use it to put the assembled packets in its message queue.
     * Everything that may take some time should be done there.
     */
    LaserTask*  laserTask; 

    /**
     * The message block is allocated and filled here, it is destructed 
     * in laserTask after the packet is processed.
     */
    ACE_Message_Block * message;

    // states for state machine
    enum {NOSYNC, SYNC, COPYMSG, LENGTH1,  ADDR};

    // important have this static !!
    // so the state of the state machine and
    // and the pointers to the data buffer of the assembled packet are
    // kept until the next call !
    int state;
    int data;
    int packData;
    unsigned char lengthlow;
    static const int BUFFSIZE = 2*1024;
    int lastfd;

    LaserStatistic *laserStatistic; 

    
private:
    //
    // these buffers are only temporary for every call to
    // handle_event, but it is inefficient to reallocate them (4K) on
    // every call to handle_event (some thousand times a second)
    // we do not need them static
    // buffer for bytes send form the laser
    unsigned char buff0[BUFFSIZE], buff1[BUFFSIZE];
    int buffPos0, buffPos1;
    int buffLen0, buffLen1;

    /**
     * termio settings of the laser interface, 
     */
    struct termios  newtio;

    /**
     * for statistics we measure the time we need per received packet.
     */
    ACE_Time_Value timeOfStx;
  };

};
#endif




