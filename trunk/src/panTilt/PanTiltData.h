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
#ifndef pantiltData_hh
#define pantiltData_hh

#include <ace/Event_Handler.h>
#include <ace/Synch.h>

namespace DpPanTilt 
{
  class Data 
  {
  public:
    Data();

    ACE_Thread_Mutex                       sync;
    ACE_Thread_Condition<ACE_Thread_Mutex> syncCond;

    bool failed;

    long panPosition;
    long tiltPosition;
    long targetPanPosition;
    long targetTiltPosition;
    long minPanPosition;
    long maxPanPosition;
    long minTiltPosition;
    long maxTiltPosition;
    bool limitbounds;
    long targetPanSpeed;
    long targetTiltSpeed;
    long panAcceleration;
    long tiltAcceleration;
    long panBaseSpeed;
    long tiltBaseSpeed;
    long maxPanSpeed;
    long minPanSpeed;
    long maxTiltSpeed;
    long minTiltSpeed;       
    bool echoing;
    bool verbose;       
    int  panHold;       
    int  tiltHold;       
    int  panMove;       
    int  tiltMove;
  };
};
#endif
