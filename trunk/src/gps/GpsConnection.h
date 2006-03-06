/*
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

#ifndef GpsConnection_h
#define GpsConnection_h

#include "gps/Parameters.h"

#include "miro/TtyConnection.h"

#include <string>

namespace GPS
{
  // forward declaration
  class EventHandler;

  /**
   * GPS::Connection
   */
  class Connection : public Miro::TtyConnection
  {
    typedef Miro::TtyConnection Super;

  public:
    //! Constructor
    Connection(ACE_Reactor * _reactor,
	    EventHandler * _eventHandler,
	    const Parameters& _parameters = *Parameters::instance());

    //! Destructor
    virtual ~Connection();
    void send(const std::string command);

  protected:
    EventHandler* eventHandler;
  };
}

#endif // GpsConnection_h
