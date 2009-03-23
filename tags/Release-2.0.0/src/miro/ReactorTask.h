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
//
// Authors: 
//   Hans Utz
//   Stefan Enderle
//   Stefan Sablatnoeg
//
#ifndef miroReactorTask_hh
#define miroReactorTask_hh

#include "Thread.h"
#include <ace/Reactor.h>
#include <ace/Sched_Params.h>

namespace Miro
{
  // forward declaration
  class Server;

  class ReactorTask : public Thread
  {
    typedef Thread  Super;
    typedef ReactorTask Self;

  public:
    ReactorTask(Miro::Server * _pServer = NULL, 
		int size = 20, 
		ACE_Sched_Params * pschedp = NULL);
    virtual ~ReactorTask();

    void shutdown();

    // methods defined by ACE_Task
    virtual int svc ();

  protected:
    Server * pServer_;

    ACE_Reactor reactor_;
    ACE_Sched_Params schedp_;
  };

  inline
  void
  ReactorTask::shutdown() {
    canceled_ = true;
  }
};

#endif




