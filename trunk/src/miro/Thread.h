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
#ifndef miroThread_hh
#define miroThread_hh

#include <ace/Sched_Params.h>
#include <ace/Task.h>

namespace Miro
{
  // forward declaration
  class Server;

  class Thread : public ACE_Task_Base
  {
    typedef ACE_Task_Base  Super;
    typedef Thread Self;

  public:
    Thread(long _flags = THR_NEW_LWP | THR_JOINABLE,
	   long _priority = ACE_DEFAULT_THREAD_PRIORITY);
    virtual ~Thread();

    void detach(int nthreads = 1);
    void cancel(bool _wait = true);
    bool canceled() const ;

    // methods defined by ACE_Task_Base
    virtual int open(void * = NULL);
    virtual int close(u_long flags = 0);

  protected:
    long flags_;
    long priority_;
    bool canceled_;
  };

  inline
  bool
  Thread::canceled() const {
    return canceled_;
  }
};
#endif




