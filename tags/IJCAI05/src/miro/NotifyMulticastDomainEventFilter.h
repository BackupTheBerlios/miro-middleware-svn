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
#ifndef NotifyMulticastDomainEventFilter_h
#define NotifyMulticastDomainEventFilter_h

/* Miro includes */
#include "NotifyMulticastEventFilter.h"

namespace Miro 
{
  namespace NMC 
  {
    class DomainEventFilter : public EventFilter 
    {
    public:
      /* constructor */
      DomainEventFilter(const std::string & _domain_name);
      
      virtual bool isAccepted(const CosNotification::StructuredEvent &_event);
      
    private :
      std::string domain_name_;
    };
  }
}
#endif // NotifyMulticastDomainEventFilter_h

