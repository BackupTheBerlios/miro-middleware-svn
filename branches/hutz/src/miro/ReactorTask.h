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

#include <ace/Reactor.h>
#include <ace/Sched_Params.h>
#include <ace/Task.h>

namespace Miro
{
  // forward declaration
  class Server;

  class ReactorTask : public ACE_Task_Base
  {
    typedef ACE_Task_Base  Super;
    typedef ReactorTask Self;

  public:
    ReactorTask(Miro::Server * _pServer = NULL, 
		int size = 20, 
		ACE_Sched_Params * pschedp = NULL);
    virtual ~ReactorTask();

    int open(void * = NULL);
    void detach(int nthreads = 1);
    void shutdown();

    // methods defined by ACE_Task
    virtual int svc ();

    // ask the task to stop itself
    void cancel();

  protected:
    Server * pServer_;

    ACE_Reactor reactor_;
    ACE_Sched_Params schedp_;

    bool shutdown_;
  };
};
#endif




