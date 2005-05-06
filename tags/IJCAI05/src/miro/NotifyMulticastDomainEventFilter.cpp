// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticastDomainEventFilter
//
//
//  (c) 2002, 2003, 2004
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Arndt Mühlenfeld <arndt.muehlenfeld@ist.tugraz.at>
//
//
//  Version:
//    1.1.0
//
//
//  Description:
//
//   EventFilter - accepts only events with appropriate domain-name. 
//
//
//  Changes:
//
//    1.1.0
//      - Initial release.
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

/* Miro includes */
#include "NotifyMulticastDomainEventFilter.h"

namespace Miro 
{
  namespace NMC
  {
    DomainEventFilter::DomainEventFilter(const std::string & _domain_name) :
      domain_name_(_domain_name) 
    {}
    
    bool
    DomainEventFilter::isAccepted(const CosNotification::StructuredEvent &_event)
    {
      return (!ACE_OS::strcmp((char const *)_event.header.fixed_header.event_type.domain_name, 
			      domain_name_.c_str()));
    }
  }
}

