// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
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
