// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////


#include "TaskBehaviour.h"

namespace Miro
{
  void 
  TaskBehaviour::open() 
  {
    Behaviour::open();

    // Activate the task
    activate();
  }

  // dummy to stop compiler warning
  int
  TaskBehaviour::open(void * p)
  {
    return ACE_Task<ACE_MT_SYNCH>::open(p);
  }

  int 
  TaskBehaviour::svc() 
  {
    while (active_)
      action();
    return 0;
  }
};
