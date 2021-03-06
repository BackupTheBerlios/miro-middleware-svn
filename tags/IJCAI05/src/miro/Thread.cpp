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
  Thread::close(u_long /* flags */ )
  {
    DBG(cout << "[Miro::Thread] ended." << std::endl);
    return 0;
  }
};
