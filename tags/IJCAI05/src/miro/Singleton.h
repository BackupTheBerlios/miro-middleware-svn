// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef miroSingleton_h
#define miroSingleton_h

#include "miro/Synch.h"
#include <ace/Singleton.h>

namespace Miro
{
  //! Templated implementation of the singleton pattern.
  /**
   * This implements a heap allocated global variable
   * with a global helper method as a accessor method.
   *
   * Example code:
   *
   * To get a pseudo global variable for class T it looks the following.
   * 
   * class T
   * { 
   *   ...
   *   Miro::Singleton<T> instance;
   * };
   *
   * Access: 
   *
   * T * t = T::instance();
   *
   * There's a paper about the singleton pattern and 
   * double checked locking on the ACE web pages.
   */
  template <class TYPE, 
	    class LOCK = ACE_Recursive_Thread_Mutex,
	    template<class TYPE, class LOCK> class ACE_SINGLETON = ACE_Singleton >
  class Singleton
  {
  public:
    //! Access operator to the global variable.
    TYPE * operator() ();
    //! Public type for friend declarations.
    typedef ACE_SINGLETON<TYPE, LOCK> ACE_Singleton_Type;
  };

  //--------------------------------------------------------------------------
  // Inlined methods
  //--------------------------------------------------------------------------
  template <class TYPE, 
	    class LOCK,
	    template<class TYPE, class LOCK> class ACE_SINGLETON >
  inline
  TYPE *
  Singleton<TYPE, LOCK, ACE_SINGLETON>::operator()() {

    return ACE_SINGLETON<TYPE, LOCK>::instance();
  }
}

#endif

