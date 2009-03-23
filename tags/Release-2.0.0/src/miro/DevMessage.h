// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
    //! We need a virtual detor as childs may define virtual methods.	  
    virtual ~DevMessage() {}
    
    //! Time the message was produced by the device.
    ACE_Time_Value  time_;
    //! Get a constant reference to the time value.
    const ACE_Time_Value& time() const { return time_;}
    //! Get a non constant reference to the time value.
    ACE_Time_Value& time() { return time_;}
  };
}

#endif






