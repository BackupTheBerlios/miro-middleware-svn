// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// for details copyright, usage and credits to other groups see Miro/COPYRIGHT
// for documentation see Miro/doc
// 
// (c) 1999,2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// Authors: 
//   Stefan Enderle, 
//   Stefan Sablatnoeg, 
//   Hans Utz
// 
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PanTiltEventHandler_h
#define PanTiltEventHandler_h

#include "miro/DevEventHandler.h"
#include "miro/Synch.h"

namespace DpPanTilt
{
  // forward declarations
  class Connection;
  class Consumer;
  class Message;

  /**
   * Event is an object wrapper for the event handler 
   * registered with the reactor.
   *
   * it is provided with a log facility and inherits from 
   * ACE_Event_Handler
   */
  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;
  public:
    /**
     * constructor does initialization
     */
    EventHandler(Connection& _connection, Consumer * _consumer);

    /**
     * destructor, needed since virtual functions are used
     */  
    virtual ~EventHandler();

    /**
     * handles file descriptor events
     */
    virtual int handle_input (ACE_HANDLE fd); // fd select
    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  protected:

    /**
     * we need a reference to the module that handles handles
     * the connection to the pantilt device.
     */
    Connection& connection_;

    /**
     * The message block is allocated and filled here, it is destructed 
     * in panTiltTask after the packet is processed.
     */
    Message * msg_;

    /**
     * states for state machine
     */
    enum { WFSTAR, WFDELIM };

    /**
     * state of the state machine
     */
    int state;

    /**
     * size of the read buffer for reiceiving packets
     * is not really relevant, packets are short
     */ 
    static const size_t BUFFSIZE=256;
  };
};
#endif




