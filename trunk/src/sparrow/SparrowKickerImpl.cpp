// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001, 2002, 2003, 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////

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
