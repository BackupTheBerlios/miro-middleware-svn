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
#include "SparrowConnection.h"

#include "miro/Exception.h"
#include "miro/StructuredPushSupplier.h"
#include "SparrowConnection.h"
#include "SparrowConnection2003.h"
#include "Parameters.h"

#include <iostream>

// #undef DEBUG

#ifdef DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif

namespace Sparrow
{
  StallImpl::StallImpl(BaseConnection * _connection,
		       Miro::StructuredPushSupplier * _pSupplier)
    throw(Miro::Exception) :
    connection(_connection),
    pSupplier(_pSupplier)
  {
    DBG(cout << "Constructing SparrowStallImpl" << endl);

    // Stall Notify Event initialization
    if (pSupplier) {
    notifyEvent.header.fixed_header.event_type.domain_name =
      CORBA::string_dup(pSupplier->domainName().c_str());
    notifyEvent.header.fixed_header.event_type.type_name =
	  CORBA::string_dup("Stall");
    notifyEvent.header.fixed_header.event_name = CORBA::string_dup("");
    notifyEvent.header.variable_header.length(0);   // put nothing here
    notifyEvent.filterable_data.length(0);          // put nothing here
    }
  }

  StallImpl::~StallImpl()
  {
    DBG(cout << "Destructing SparrowStallImpl" << endl);
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
       rvalue = false;//((Connection2003 *) connection)->isStalled();
    else
       rvalue = ((Connection *) connection)->isStalled();

    return rvalue;
  }

  void
  StallImpl::unstall() throw()
  {
    //connection.unstall();
    if(!Parameters::instance()->sparrow2003)
           ((Connection *) connection)->unstall();
  }
};
