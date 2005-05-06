// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////
#ifndef Sparrow_AliveCollector_h
#define Sparrow_AliveCollector_h

#include "miro/Synch.h"
#include <ace/Time_Value.h>

namespace Sparrow
{
  class AliveCollector
  {
  public:

    AliveCollector();

    bool kickAlive();
    bool panAlive();
    bool infrared1Alive();
    bool infrared2Alive();
    bool motorAlive();

    void setLastKickAlive(ACE_Time_Value const& _last);
    void setLastPanAlive(ACE_Time_Value const& _last);
    void setLastInfrared1Alive(ACE_Time_Value const& _last);
    void setLastInfrared2Alive(ACE_Time_Value const& _last);
    void setLastMotorAlive(ACE_Time_Value const& _last);
    
  protected:
    Miro::Mutex mutex_;

    ACE_Time_Value lastKickAlive_;
    ACE_Time_Value lastPanAlive_;
    ACE_Time_Value lastInfrared1Alive_;
    ACE_Time_Value lastInfrared2Alive_;
    ACE_Time_Value lastMotorAlive_;

    static ACE_Time_Value const TIMEOUT;
  };
}

#endif // Sparrow_AliveCollector_h


