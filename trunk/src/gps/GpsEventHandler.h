/* -*- C++ -*-
 *
 * This file is part of Miro (The Middleware for Robots)
 * Copyright (C) 2005 Ingo van Lil <ingo@vanlil.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id$
 */

#ifndef GpsEventHandler_h
#define GpsEventHandler_h

#include "miro/DevEventHandler.h"

namespace GPS
{
  // forward declarations
  class Connection;
  class Parameters;

  class EventHandler : public Miro::DevEventHandler
  {
    typedef Miro::DevEventHandler Super;

  public:
    EventHandler(Miro::DevConsumer * _consumer);
    virtual ~EventHandler();

    virtual int handle_input(ACE_HANDLE fd);

  private:
    enum {WAIT_SOS, COPY_MSG, WAIT_LF} state_;
    static const int BUFSIZE = 128;
    char buffer_[BUFSIZE];
    char sentence_[BUFSIZE];
    int pos_;
  };
}

#endif // GpsEventHandler_h
