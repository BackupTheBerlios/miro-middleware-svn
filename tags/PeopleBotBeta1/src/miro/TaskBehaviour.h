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
#ifndef TaskBehaviour_h
#define TaskBehaviour_h

#include "Behaviour.h"
#include <ace/Task.h>

namespace Miro
{
  class TaskBehaviour : 
    public Behaviour, 
    public ACE_Task<ACE_MT_SYNCH>
  {
    typedef Behaviour Super;
  public:
    void open();
    int svc();

  private:
    int open(void*);
  };
};
#endif
