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
#ifndef BAP_TaskBehaviour_h
#define BAP_TaskBehaviour_h

#include "Behaviour.h"
#include <ace/Task.h>

namespace Miro
{
  namespace BAP
  {
    class TaskBehaviour : 
      public Behaviour, 
      public ACE_Task<ACE_MT_SYNCH>
    {
      typedef Behaviour Super;
    public:
      virtual void open();
      virtual int svc();
      
    private:
      virtual int open(void*);
    };
  }
}
#endif // BAP_TaskBehaviour_h
