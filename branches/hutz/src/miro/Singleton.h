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
#ifndef miroSingleton_hh
#define miroSingleton_hh

#include "miro/Synch.h"

namespace Miro
{
  template <class TYPE>
  class Singleton
  {
  public:
    TYPE * operator() ();

  protected:
    static TYPE * instance_;
    static Mutex lock_;
  };
};

template <class TYPE>
inline
TYPE *
Miro::Singleton<TYPE>::operator() (){
  // Perform the Double-Checed Locking to
  // ensure proper initialization.
  if (instance_ == 0) {
    Guard guard(lock_);
    if (instance_ == 0)
      instance_ = new TYPE;
  };
  return instance_;
}

// Pointer to the Singleton instance.
template <class TYPE>
TYPE * Miro::Singleton<TYPE>::instance_ = 0;

template <class TYPE>
Miro::Mutex Miro::Singleton<TYPE>::lock_;

#endif

