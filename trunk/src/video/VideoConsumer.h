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
#ifndef VideoConsumer_h
#define VideoConsumer_h

#include "miro/Synch.h"
#include "miro/Thread.h"
#include "idl/TimeC.h"

namespace Video
{
  // forward declerations
  class Device;

  /**
   * Class for using the Video robot
   */
  class Consumer : public Miro::Thread
  {
    typedef Miro::Thread Super;
    
  public:
    Consumer(Device& _device, 
	     ACE_Sched_Params * pschedp = NULL);
    ~Consumer();

    virtual int svc();

    void setMaxSynchJitter(ACE_Time_Value const& _maxJitter) throw();
    
  protected:
    Device& videoDevice;
    ACE_Sched_Params schedp_;
    ACE_Time_Value maxSynchJitter_;
  };

  inline
  void
  Consumer::setMaxSynchJitter(ACE_Time_Value const& _maxJitter) throw() {
    maxSynchJitter_ = _maxJitter;
  }
}
#endif

