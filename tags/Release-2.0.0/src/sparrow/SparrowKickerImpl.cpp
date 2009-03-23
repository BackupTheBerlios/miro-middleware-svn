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
#include "SparrowKickerImpl.h"
#include "SparrowBaseConnection.h"
#include "SparrowConnection2003.h"
#include "SparrowConnection.h"
#include "Parameters.h"

#include "miro/Log.h"

namespace Sparrow
{
  KickerImpl::KickerImpl(BaseConnection * _connection) :
    connection_(_connection)
  {
    MIRO_LOG_CTOR("Sparrow:KickerImpl");
  }

  KickerImpl::~KickerImpl()
  {
    MIRO_LOG_DTOR("Sparrow:KickerImpl");
  }

  //
  // IDL interface implementation

  //--------------------------------------------------------------------------
  // Kicker interface implementation
  //--------------------------------------------------------------------------

  void
  KickerImpl::kick(const Miro::TimeIDL& time) throw()
  {
    if (Sparrow::Parameters::instance()->sparrow2003) {
      Connection2003 * connection = 
	dynamic_cast<Connection2003 *> (connection_);
      MIRO_ASSERT(connection != NULL);

      connection->kick(50, (unsigned char)(time.usec/1000));
    }
    else {
      Connection * connection = 
	dynamic_cast<Connection *> (connection_);
      MIRO_ASSERT(connection != NULL);

      connection->kick(time.sec * 1000 + time.usec / 1000);
    }
  }
}
