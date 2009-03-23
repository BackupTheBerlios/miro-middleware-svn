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
#ifndef BAP_TaskBehaviour_h
#define BAP_TaskBehaviour_h

#include "Behaviour.h"
#include <ace/Task.h>

namespace Miro
{
  namespace BAP
  {
    class TaskBehaviour : 
      public Behaviour, 
      public ACE_Task<ACE_MT_SYNCH>
    {
      typedef Behaviour Super;
    public:
      virtual void open();
      virtual int svc();
      
    private:
      virtual int open(void*);
    };
  }
}
#endif // BAP_TaskBehaviour_h
