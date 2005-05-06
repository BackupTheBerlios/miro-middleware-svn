// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "TaskBehaviour.h"
#include "BehaviourParameters.h"

namespace Miro
{
  namespace BAP
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
      while (active_) {
	// clean up dangling behaviour parameters
	// left over from ActionPattern::setBehaviourParameters()    
	delete oldParams_;
	oldParams_ = NULL;
	
	action();
      }
      return 0;
    }
  }
}
