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
#ifndef DevConsumer_h
#define DevConsumer_h

namespace Miro
{
  // forward declarations
  class DevMessage;

  /**
   * Consumer Class.
   *
   * @author Hans Utz
   */
  class DevConsumer
  {
  public:
    DevConsumer();
    virtual ~DevConsumer();

  /**
   * Handle dispatched Message.
   *
   * This method is called for each message extracted from the 
   * file descriptor of thdevice.
   *
   * Overwrite this method for your own device consumer. You can
   * pass any non handled message to the parent handler. This
   * handleMessage implementation just prints a warning about an 
   * unhandled message to cerr.
   */
    virtual void handleMessage(const DevMessage * message);
  };
}
#endif




