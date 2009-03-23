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
#include "TaskBehaviour.h"
#include "BehaviourParameters.h"

namespace Miro
{
  namespace BAP
  {
    void 
    TaskBehaviour::open() 
    {
      Behaviour::open();
      
      // Activate the task
      activate();
    }

    // dummy to stop compiler warning
    int
    TaskBehaviour::open(void * p)
    {
      return ACE_Task<ACE_MT_SYNCH>::open(p);
    }

    int 
    TaskBehaviour::svc() 
    {
      while (active_) {
	// clean up dangling behaviour parameters
	// left over from ActionPattern::setBehaviourParameters()    
	delete oldParams_;
	oldParams_ = NULL;
	
	action();
      }
      return 0;
    }
  }
}
