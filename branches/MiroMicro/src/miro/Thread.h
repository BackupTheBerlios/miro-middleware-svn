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
#ifndef miroThread_hh
#define miroThread_hh

#include <ace/Sched_Params.h>
#include <ace/Task.h>

namespace Miro
{
  // forward declaration
  class Server;

  class Thread : public ACE_Task_Base
  {
    typedef ACE_Task_Base  Super;
    typedef Thread Self;

  public:
    Thread(long _flags = THR_NEW_LWP | THR_JOINABLE,
           long _priority = ACE_DEFAULT_THREAD_PRIORITY);
    virtual ~Thread();

    virtual void detach(int nthreads = 1);
    virtual void cancel(bool _wait = true);
    bool canceled() const ;

    // methods defined by ACE_Task_Base
    virtual int open(void * = NULL);
    virtual int close(u_long flags = 0);

  protected:
    long flags_;
    long priority_;
    bool canceled_;
  };

  inline
  bool
  Thread::canceled() const
  {
    return canceled_;
  }
}
#endif




