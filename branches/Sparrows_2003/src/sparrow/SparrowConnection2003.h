// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef SparrowConnection2003_h
#define SparrowConnection2003_hh

#include "SparrowDevice2003.h"
#include "Parameters.h"

#include "SparrowBaseConnection.h"         // CanConnection, CanMessageClass

//------------------------ SparrowClass ---------------------------//

namespace Sparrow
{
  // forward declaration
  class Consumer2003;
  class EventHandler2003;

  /**
   * Class for using the Sparrow robot
   */
  class Connection2003 : public BaseConnection
  {
    typedef BaseConnection Super;

  public:
    Connection2003(ACE_Reactor * _reactor,
	       Miro::DevEventHandler * _eventHandler,
	       Consumer2003 * _consumer);
    virtual ~Connection2003();

    void init();

    void infraredGet(unsigned short msec, unsigned short times);
    bool infraredAlive();


  protected:

    Consumer2003 * consumer;
    EventHandler2003 * eventHandler;

  public:
    int boardReply; // for watchdog / init
  };
};

#endif

