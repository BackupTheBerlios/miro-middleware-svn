// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2005
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef PanTiltBase_h
#define PanTiltBase_h

#include "miro/Synch.h"

namespace Miro
{

  //! Implementation of a dummy PanTilt interface.
  /**
   * This class offers a generic implementation for the PanTilt
   *
   * @author Guillem Pagès Gassull
   */
  class  PanTiltBase
  {
  protected:
    ACE_Recursive_Thread_Mutex mutex_;
  };
}

#endif // PanTiltImpl_h
