// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef DevMessage_h
#define DevMessage_h

#include <cmath>  // work around for ACE_OS bug with headers
#include <ace/OS.h>

namespace Miro
{
  /**
   * The mother of all messages.
   *
   * Derive your message class, the one which is shipped via a 
   * ACE_Message_Block to the consumer @ref Task from this class.
   *
   * It only contains a time stamp, which you have to set yourself
   * when you feel it is time. But the @ref Task class assumes the
   * data coming via its message queue to be a descendant of this
   * class and the only way to stop him would have been, to 
   * templatize @ref Connection, @ref Task and @ref Event.
   *
   * @author Hans Utz
   */
  class DevMessage 
  {
  public:
    //! Time the message was produced by the device.
    ACE_Time_Value  time_;
    //! Get a constant reference to the time value.
    const ACE_Time_Value& time() const { return time_;}
    //! Get a non constant reference to the time value.
    ACE_Time_Value& time() { return time_;}
  };
}

#endif






