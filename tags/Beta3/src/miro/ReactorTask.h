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




