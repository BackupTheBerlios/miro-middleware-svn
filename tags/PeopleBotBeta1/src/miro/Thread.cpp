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


#include "miro/Thread.h"
#include "miro/Exception.h"

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
  Thread::Thread(ACE_Sched_Params *pschedp) :  
    Super(),
    schedp_(ACE_SCHED_OTHER, 0),
    canceled_(true)
  {
    DBG(cout << "Constructing [Miro::Thread]." << endl);
    if (pschedp) 
      schedp_ = (*pschedp);
  
  }

  Thread::~Thread()
  {
    DBG(cout << "Destructing [Miro::Thread]." << endl);
  }

  int
  Thread::open(void *)
  {
    canceled_ = false;

    if (activate(THR_NEW_LWP | THR_JOINABLE, 1) != 0)
      throw Exception("[Miro::Thread] Cannot activate client thread");

    // set the given thread scheduling policy
    if (ACE_OS::sched_params(schedp_) == -1) {
      DBG(cerr << "[Miro::Thread] Could not set sched parameters. (maybe suid root is missing)" << endl);
    }

    return 0;
  }

  void
  Thread::detach(int _nthreads)
  {
    canceled_ = false;

    if (activate(THR_NEW_LWP | THR_JOINABLE, _nthreads) != 0)
      throw Exception("[Miro::Thread] Cannot activate client threads");
  }

  void
  Thread::cancel(bool _wait) 
  {
    DBG(cout << "[Miro::Thread] shutdown." << endl);
    canceled_ = true;
    if (_wait)
      wait();
  }

  int
  Thread::close(u_long /* flags */ )
  {
    DBG(cout << "[Miro::Thread] ended." << endl);
    return 0;
  }
};
