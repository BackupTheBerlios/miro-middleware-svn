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


#include "miro/ExceptionC.h"
#include "miro/Server.h"
#include "miro/ReactorTask.h"
#include "miro/Exception.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
using std::cout;
using std::cerr;

#else
#define DBG(x)
#endif

using Miro::Exception;

namespace Miro
{
  ReactorTask::ReactorTask(Server * _pServer, 
			   int size,
			   ACE_Sched_Params *pschedp) :  
    Super(),
    pServer_(_pServer),
    reactor_(),
    schedp_(ACE_SCHED_OTHER, 0),
    shutdown_(false)
  {
    DBG(cout << "Constructing [Miro::ReactorTask]." << endl);
    reactor(&reactor_);
    reactor_.open(size);
    if (pschedp) 
      schedp_ = (*pschedp);
  
  }

  ReactorTask::~ReactorTask()
  {
    DBG(cout << "Destructing [Miro::ReactorTask]." << endl);
    reactor_.close();
    reactor(NULL);
  }

  int
  ReactorTask::open(void *)
  {
    if (activate(THR_NEW_LWP | THR_JOINABLE, 1) != 0)
      throw Exception("[Miro::ReactorTask] Cannot activate client thread");

    return 0;
  }

  void
  ReactorTask::detach(int nthreads)
  {
    if (activate(THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
      throw Exception("[Miro::ReactorTask] Cannot activate client threads");
  }

  //
  // Now the svc() method where everything interesting happens.
  //
  int 
  ReactorTask::svc()
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) [Miro::ReactorTask] 0x%x starts in thread %u\n", (void *) this, ACE_Thread::self()));

    ACE_Time_Value timeout(0, 800000); // wait for 1/10 sec 
    ACE_Time_Value delta;              // uninitialized time value

    // set the given thread scheduling policy

    if (ACE_OS::sched_params(schedp_) == -1) {
      DBG(cerr << "[MiroreactorTask] Could not set sched parameters." << endl 
	       << "(maybe suid root is missing)" << endl);
    }

    // set the thread to be the owner of the reactor, 
    //otherwise we will get errors
    reactor_.owner(ACE_OS::thr_self());

    while (!shutdown_) {
      // set delta to timeout
      // handle_events decrements the timeout
      delta = timeout;
      // trigger message handling
      try {
	reactor_.handle_events(delta);
      }
      catch (const ETimeOut& e) {
	cerr << "ReactorTask.handleMessage() - Miro Timeout exception: " << e << endl;
	if(pServer_)
	    pServer_->shutdown();
      }
      catch (const EDevIO& e) {
	cerr << "ReactorTask.handleMessage() - Miro Device I/O exception: " << e << endl;
	if(pServer_)
	  pServer_->shutdown();
      }
      catch (const EOutOfBounds& e) {
	cerr << "ReactorTask.handleMessage() - Miro out of bounds exception: " << e << endl;
	if(pServer_)
	  pServer_->shutdown();
      }
      catch (const CORBA::Exception& e) {
	cerr << "ReactorTask.handleMessage() - Uncaught CORBA exception: " << e << endl;
	if(pServer_)
	  pServer_->shutdown();
      }
      catch(const Miro::CException& e) {
	cerr << "ReactorTask.handleMessage() - c exception: " << e << endl;
	if(pServer_)
	  pServer_->shutdown();
      }
      catch(const Miro::Exception& e) {
	cerr << "ReactorTask.handleMessage() - exception: " << e << endl;
	if(pServer_)
	  pServer_->shutdown();
      }
      catch(...) {
	cerr << "ReactorTask.handleMessage() - unknown exception: " << endl;
	if(pServer_)
	  pServer_->shutdown();
      }
    } 
    return 0;
  }

  void
  ReactorTask::shutdown() 
  {
    shutdown_ = true;
  }

  void
  ReactorTask::cancel()
  {
    shutdown();

    // Wait for the threads of the Task to exit.
    // It is rather rude to let the Task go out of scope without doing 
    // this first.
    wait();

    DBG(cout << "[Miro::ReactorTask] ended." << endl);
  }
};
