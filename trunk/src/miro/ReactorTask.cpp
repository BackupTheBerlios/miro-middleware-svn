// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "idl/ExceptionC.h"
#include "miro/ReactorTask.h"
#include "miro/Server.h"
#include "miro/Exception.h"
#include <miro/Log.h>

using Miro::Exception;

namespace Miro
{
  using ::operator<<;


  ReactorTask::ReactorTask(Server * _pServer, 
			   int size,
			   ACE_Sched_Params *pschedp) :  
    Super(),
    pServer_(_pServer),
    reactor_(),
    schedp_(ACE_SCHED_OTHER, 0)
  {
    MIRO_DBG(MIRO,LL_CTOR_DTOR,"Constructing [Miro::ReactorTask].\n");
    reactor(&reactor_);
    reactor_.open(size);
    if (pschedp) 
      schedp_ = (*pschedp);
  }

  ReactorTask::~ReactorTask()
  {
    MIRO_DBG(MIRO,LL_CTOR_DTOR,"Destructing [Miro::ReactorTask].\n");
    reactor_.close();
    reactor(NULL);
    MIRO_LOG(LL_NOTICE,"Reactor canceled.\n");
  }

  //
  // Now the svc() method where everything interesting happens.
  //
  int 
  ReactorTask::svc()
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) [Miro::ReactorTask] 0x%x starts in thread %u\n", (void *) this, ACE_Thread::self()));

    ACE_Time_Value timeout(0, 100000); // wait for 100 msec 
    ACE_Time_Value delta;              // uninitialized time value

    // set the given thread scheduling policy

    if (ACE_OS::sched_params(schedp_) == -1) {
      MIRO_LOG_OSTR(LL_ERROR,"[Miro::ReactorTask] Could not set sched parameters." << std::endl 
		<< "[Miro::ReactorTask] Maybe suid root is missing." << std::endl
		<< "[Miro::ReactorTask] Will work on default scheduling policy." << std::endl);
    }

    // set the thread to be the owner of the reactor, 
    //otherwise we will get errors
    reactor_.owner(ACE_OS::thr_self());

    while (!canceled()) {
      // set delta to timeout
      // handle_events decrements the timeout
      delta = timeout;
      // trigger message handling
      try {
	reactor_.handle_events(delta);
      }
      catch (const ETimeOut& e) {
        MIRO_LOG_OSTR(LL_CRITICAL,"ReactorTask.handleMessage() - Miro Timeout exception: " << e << std::endl);
	if(pServer_)
	    pServer_->shutdown();
      }
      catch (const EDevIO& e) {
        MIRO_LOG_OSTR(LL_CRITICAL,"ReactorTask.handleMessage() - Miro Device I/O exception: " << e << std::endl);
	if(pServer_)
	  pServer_->shutdown();
      }
      catch (const EOutOfBounds& e) {
        MIRO_LOG_OSTR(LL_CRITICAL,"ReactorTask.handleMessage() - Miro out of bounds exception: " << e << std::endl);
	if(pServer_)
	  pServer_->shutdown();
      }
      catch (const CORBA::Exception& e) {
        MIRO_LOG_OSTR(LL_CRITICAL,"ReactorTask.handleMessage() - Uncaught CORBA exception: " << e << std::endl);
	if(pServer_)
	  pServer_->shutdown();
      }
      catch(const Miro::CException& e) {
        MIRO_LOG_OSTR(LL_CRITICAL,"ReactorTask.handleMessage() - c exception: " << e << std::endl);
	if(pServer_)
	  pServer_->shutdown();
      }
      catch(const Miro::Exception& e) {
        MIRO_LOG_OSTR(LL_CRITICAL,"ReactorTask.handleMessage() - exception: " << e << std::endl);
	if(pServer_)
	  pServer_->shutdown();
      }
      catch(...) {
        MIRO_LOG_OSTR(LL_CRITICAL,"ReactorTask.handleMessage() - unknown exception: " << std::endl);
	if(pServer_)
	  pServer_->shutdown();
      }
    } 
    return 0;
  }
}
