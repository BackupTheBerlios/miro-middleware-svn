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
#ifndef miroTask_hh
#define miroTask_hh

#include <ace/Task.h>

namespace Miro
{
  /** 
   * Wrapper for ACE_Task.
   *
   * This is just a simple wrapper to the ACE_Task template class
   * to shorten its declaration. Read the ACE documentation for details
   * of this class.
   *
   * Basically, by calling open() on an instance of this class or its
   * derivates, a single new thread is spawned which executes the
   * svc() method of this class - which you therefore have to override,
   * to get your stuff done.
   */
  class Task : public ACE_Task < ACE_MT_SYNCH >
  {
    typedef ACE_Task<ACE_MT_SYNCH> super;
    typedef Task self;

  public:
    Task();
    virtual ~Task();

    /**
     * Opens one single new thread, which enters the svc() method.
     *
     * The parameter is actually just ignored, but it is best 
     * to supply NULL as a matter of style.
     *
     * If you want to open more than one thread, overwrite this method.
     */
    virtual int open(void *);
  };
};

#endif




