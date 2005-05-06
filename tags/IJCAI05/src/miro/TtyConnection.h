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
#ifndef TtyConnection_h
#define TtyConnection_h

#include <ace/DEV_Connector.h>
#include <ace/TTY_IO.h>

// forward declarations
class ACE_Reactor;

namespace Miro
{
  // forward declarations
  class DevEventHandler;
  class TtyParameters;

  /**
   * The base class for a device handler.
   *
   * This is the
   */
  class TtyConnection
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
    TtyConnection(ACE_Reactor * _reactor,
		  DevEventHandler * _eventHandler,
		  const TtyParameters& _parameters);
    /**
     * Destructor.
     *
     * It deregisters the event handler from the reactor,
     * stops the consumer task and closes the file descriptor.
     */
    virtual ~TtyConnection();
    ACE_TTY_IO ioBuffer;

  protected:
    /** Pointer to the ACE_Reactor the @ref Event is registered to. */
    ACE_Reactor * reactor_;   // for event handling (file descriptors)
    /**
     * The consumer @ref DevEventHandler instance.
     */
    DevEventHandler * eventHandler_;
    ACE_DEV_Addr ttyName_;
    ACE_DEV_Connector connector_;
  };
}

#endif




