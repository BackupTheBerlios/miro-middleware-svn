// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef DevConnection_h
#define DevConnection_h

#include <ace/DEV_Connector.h>

// forward declarations
class ACE_Reactor;

namespace Miro 
{
  // forward declarations
  class DevEventHandler;
  class DevParameters;

  /**
   * The base class for a device handler.
   *
   * @author Hans Utz
   */
  class DevConnection
  {
  public:
    /**
     * Constructor.
     *
     * It stores the reactor, event and task,
     * opens the file pointed to by deviceName
     * and registers the Event::handle_event method at the reactor.
     *
     * @param reactor_
     *        Pointer to a ACE_Reactor.
     * @param event_
     *        Pointer to an instance of your Event class, which is
     *        derived from @ref Event.
     * @param task_
     *        Pointer to an instance of your Task class, which is
     *        derived from @ref Task.
     * @param deviceName
     *        Name of the device to open.
     */
    DevConnection(ACE_Reactor * _reactor, 
		  DevEventHandler * _event,
		  const DevParameters&  _parameters);
    /**
     * Destructor.
     *
     * It deregisters the event handler from the reactor,
     * stops the consumer task and closes the file descriptor.
     */
    virtual ~DevConnection();
  
  protected:
    /** Pointer to the ACE_Reactor the @ref Event is registered to. */
    ACE_Reactor* reactor;   // for event handling (file descriptors)
    /**
     * The consumer @ref DevEventHandler instance.
     */
    DevEventHandler* eventHandler;

    ACE_DEV_Addr devName;
    ACE_DEV_IO ioBuffer;
    ACE_DEV_Connector connector;

    /**
     * The Id of the @ref DevEventHandler at the ACE_Reactor.
     */
    int selectHandlerId;
    ACE_Time_Value tv;
  };
};

#endif




