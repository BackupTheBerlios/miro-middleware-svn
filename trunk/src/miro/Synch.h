// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroSynch_hh
#define miroSynch_hh

#include <ace/Synch.h>
// Hack for TAO <= x.2.1 compability
#include <ace/CORBA_macros.h>

#ifndef ACE_ENV_ARG_DECL_WITH_DEFAULTS
#define ACE_ENV_ARG_DECL_WITH_DEFAULTS \
 , CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ()
#endif
#ifndef ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
#define ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS \
 CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ()
#endif
#ifndef ACE_ENV_ARG_DECL_NOT_USED
#define ACE_ENV_ARG_DECL_NOT_USED \
 , CORBA::Environment &
#endif
#ifndef ACE_ENV_SINGLE_ARG_DECL_NOT_USED
#define ACE_ENV_SINGLE_ARG_DECL_NOT_USED \
 CORBA::Environment &
#endif
#ifndef ACE_ENV_ARG_PARAMETER
#define ACE_ENV_ARG_PARAMETER \
  ,  TAO_default_environment ()
#endif

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

  typedef ACE_RW_Thread_Mutex                  RWMutex;
  typedef ACE_Read_Guard<ACE_RW_Thread_Mutex>  ReadGuard;
  typedef ACE_Write_Guard<ACE_RW_Thread_Mutex> WriteGuard;
}

#endif




