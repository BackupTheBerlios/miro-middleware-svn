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
#include "Thread.h"
#include "Exception.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
using std::cout;

#else
#define DBG(x)
#endif

namespace Miro
{
  Thread::Thread(long _flags, long _priority) :
      Super(),
      flags_(_flags),
      priority_(_priority),
      canceled_(true)
  {
    DBG(cout << "Constructing [Miro::Thread]." << std::endl);
  }

  Thread::~Thread()
  {
    DBG(cout << "Destructing [Miro::Thread]." << std::endl);
  }

  int
  Thread::open(void *)
  {
    canceled_ = false;

    if (activate(flags_, 1, 0, priority_) != 0)
      throw Exception("[Miro::Thread] Cannot activate client thread");

    return 0;
  }

  void
  Thread::detach(int _nthreads)
  {
    canceled_ = false;

    if (activate(flags_, _nthreads, 0, priority_) != 0)
      throw Exception("[Miro::Thread] Cannot activate client thread");
  }

  void
  Thread::cancel(bool _wait)
  {
    DBG(cout << "[Miro::Thread] shutdown." << std::endl);
    if (!canceled_) {
      canceled_ = true;
      if (_wait)
        wait();
    }
  }

  int
  Thread::close(u_long /* flags */)
  {
    DBG(cout << "[Miro::Thread] ended." << std::endl);
    return 0;
  }
};
