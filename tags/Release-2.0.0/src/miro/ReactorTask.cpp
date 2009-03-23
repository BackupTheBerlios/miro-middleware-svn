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
    MIRO_LOG_CTOR("Miro::ReactorTask");
    reactor(&reactor_);
    reactor_.open(size);
    if (pschedp) 
      schedp_ = (*pschedp);
  }

  ReactorTask::~ReactorTask()
  {
    MIRO_LOG_DTOR("Miro::ReactorTask");
    reactor_.close();
    MIRO_LOG(LL_NOTICE, "Reactor closed.");
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
