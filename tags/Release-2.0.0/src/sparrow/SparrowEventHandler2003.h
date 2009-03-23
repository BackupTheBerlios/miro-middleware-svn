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
#ifndef SparrowEventHandler2003_h
#define SparrowEventHandler2003_h

#include <ace/Event_Handler.h>

namespace Sparrow
{
  // forward declerations
  class Connection2003;

  class EventHandler2003 : public ACE_Event_Handler
  {
  public:
    EventHandler2003(Connection2003& _connection);
    virtual ~EventHandler2003();

    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

  protected:
    Connection2003& connection_;
  };
}

#endif // SparrowEventHandler2003_h




