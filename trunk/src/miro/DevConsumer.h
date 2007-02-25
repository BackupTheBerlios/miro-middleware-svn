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
#ifndef DevConsumer_h
#define DevConsumer_h

namespace Miro
{
  // forward declarations
  class DevMessage;

  /**
   * Consumer Class.
   *
   * @author Hans Utz
   */
  class DevConsumer
  {
  public:
    DevConsumer();
    virtual ~DevConsumer();

  /**
   * Handle dispatched Message.
   *
   * This method is called for each message extracted from the 
   * file descriptor of thdevice.
   *
   * Overwrite this method for your own device consumer. You can
   * pass any non handled message to the parent handler. This
   * handleMessage implementation just prints a warning about an 
   * unhandled message to cerr.
   */
    virtual void handleMessage(const DevMessage * message);
  };
}
#endif




