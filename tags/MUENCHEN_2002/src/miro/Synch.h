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
#ifndef miroSynch_hh
#define miroSynch_hh

#include <ace/Synch.h>

// These are just some simple typedefs to some ACE_Synch templates
// to shorten declerations. Read the ACE documentation for details
// of this class.
//
// If you feel some other ACE_Synch types
// to be better suited for your purposes, just go ahead.

namespace Miro
{
  typedef ACE_Thread_Mutex            Mutex;
  typedef ACE_Condition_Thread_Mutex  Condition;
  typedef ACE_Guard<ACE_Thread_Mutex> Guard;

  typedef ACE_RW_Thread_Mutex         RWMutex;
  typedef ACE_Read_Guard<RWMutex>     ReadGuard;
  typedef ACE_Write_Guard<RWMutex>    WriteGuard;
};

#endif




