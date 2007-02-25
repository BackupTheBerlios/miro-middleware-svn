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


