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
#include "DeviceAsynchBufferSet.h"

namespace Video
{
  DeviceAsynchBufferSet::DeviceAsynchBufferSet(Device const * _device,
					       Miro::Mutex * _mutex,
					       Miro::Condition * _condition) :
    device_(_device),
    connected_(false),
    connections_(0),
    needsConnectivityUpdate_(false),
    mutex_(_mutex),
    condition_(_condition),
    upcomingSet_(4),
    currentSet_(4),
    lockedSet_(4)
  {
    for (unsigned int i = 0; i < 3; ++i) {
      stamp_[i] = ACE_Time_Value::zero;
    }
  }

  void
  DeviceAsynchBufferSet::synchMode(bool _synchWithCurrent,
				   bool _synchAllNew) throw() 
  {
    synchWithCurrent_ = _synchWithCurrent;
    synchAllNew_ = _synchAllNew;
  }
}
