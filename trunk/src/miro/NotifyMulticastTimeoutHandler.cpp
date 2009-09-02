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
//
// Authors:
//   Philipp Baer
//
#include "NotifyMulticastDefines.h"
#include "NotifyMulticastTimeoutHandler.h"
#include "NotifyMulticastReceiver.h"

#include "Log.h"

namespace Miro
{
  namespace NMC {
    /**
     *     Timeout handler for periodical updates on fragment registry.
     *
     *   Parameters:
     *     _receiver: Pointer to receiver instance
     */
    TimeoutHandler::TimeoutHandler(Receiver *_receiver) :
        receiver_(_receiver)
    {
      MIRO_LOG_CTOR("NMC:TimeoutHandler");
    }

    /**
     *     Called timer thread
     *
     *   Parameters:
     *     _tv:  time value for timeout
     *     _act: generic pointer (to whatever)
     */
    int
    TimeoutHandler::handle_timeout(const ACE_Time_Value &_tv,
                                   const void           *_act)
    {
      if (receiver_)
        this->receiver_->handle_timeout(_tv, _act);

      return 0;
    }
  }
}

