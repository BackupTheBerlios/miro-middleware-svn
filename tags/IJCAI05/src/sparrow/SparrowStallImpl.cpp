// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 1999, 2000, 2001
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
//
//////////////////////////////////////////////////////////////////////////////


#include "SparrowStallImpl.h"
#include "SparrowBaseConnection.h"
#include "SparrowConnection.h"
#include "SparrowConnection2003.h"
#include "Parameters.h"

#include "miro/StructuredPushSupplier.h"
#include "miro/Log.h"

namespace Sparrow
{
  StallImpl::StallImpl(BaseConnection * _connection,
		       Miro::StructuredPushSupplier * _pSupplier) :
    connection_(_connection),
    pSupplier(_pSupplier)
  {
    MIRO_LOG_CTOR("Sparrow::StallImpl");

    // Stall Notify Event initialization
    if (pSupplier) {
      notifyEvent.header.fixed_header.event_type.domain_name =
	CORBA::string_dup(pSupplier->domainName().c_str());
      notifyEvent.header.fixed_header.event_type.type_name =
	CORBA::string_dup("Stall");
      notifyEvent.header.fixed_header.event_name = CORBA::string_dup("");
      notifyEvent.header.variable_header.length(0);   // put nothing here
      notifyEvent.filterable_data.length(0);          // put nothing here

      CosNotification::EventTypeSeq offers;
      offers.length(1);
      offers[0] = notifyEvent.header.fixed_header.event_type;
      pSupplier->addOffers(offers);
    }
  }

  StallImpl::~StallImpl()
  {
    MIRO_LOG_DTOR("Sparrow::StallImpl");
  }

  void
  StallImpl::pushEvent() const
  {
    if (pSupplier)
      pSupplier->sendEvent(notifyEvent);
  }

  //
  // IDL interface implementation

  //--------------------------------------------------------------------------
  // Stall interface implementation
  //--------------------------------------------------------------------------

  CORBA::Boolean
  StallImpl::isStalled() throw()
  {
    CORBA::Boolean rvalue;
    if(Parameters::instance()->sparrow2003)
       rvalue = false;
    else {
      Connection * connection = 
	dynamic_cast<Connection *> (connection_);
      MIRO_ASSERT(connection != NULL);

      rvalue = connection->isStalled();
    }

    return rvalue;
  }

  void
  StallImpl::unstall() throw()
  {
    if(!Parameters::instance()->sparrow2003) {
      Connection * connection = 
	dynamic_cast<Connection *> (connection_);
      MIRO_ASSERT(connection != NULL);

      connection->unstall();
    }
  }
}
