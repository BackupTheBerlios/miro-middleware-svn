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
#ifndef miroSingleton_h
#define miroSingleton_h

#include "miro/Synch.h"

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
  template <class TYPE>
  class Singleton
  {
  public:
    //! Access operator to the global variable.
    TYPE * operator() ();

  protected:
    //! Pointer to the global variable.
    static TYPE * instance_;
    //! Lock for the double checked locking pattern.
    static Mutex lock_;
  };

  //--------------------------------------------------------------------------
  // Inlined methods
  //--------------------------------------------------------------------------
  template <class TYPE>
  inline
  TYPE *
  Singleton<TYPE>::operator()() {
    // Perform the Double-Checed Locking to
    // ensure proper initialization.
    if (instance_ == 0) {
      Guard guard(lock_);
      if (instance_ == 0)
	instance_ = new TYPE;
    };
    return instance_;
  }

  //--------------------------------------------------------------------------
  // Static template data member instances.
  //--------------------------------------------------------------------------
  template <class TYPE>
  TYPE * Singleton<TYPE>::instance_ = 0;

  template <class TYPE>
  Mutex Singleton<TYPE>::lock_;
};

#endif

