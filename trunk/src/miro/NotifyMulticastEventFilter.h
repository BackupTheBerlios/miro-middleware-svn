// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticastEventFilter
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
//   isAccepted() is called when new event is received from the
//   EventChannel. 
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

#ifndef NotifyMulticastEventFilter_h
#define NotifyMulticastEventFilter_h

/* Miro includes */
#include "StructuredPushConsumer.h"

namespace Miro {

    namespace NotifyMulticast {

        class EventFilter {

            public:
                /* Default constructor */
                EventFilter() {};
		
                /* Default destructor */
                virtual ~EventFilter() {};

                virtual bool isAccepted(const CosNotification::StructuredEvent &_event) = 0;
        };
    };
};

#endif /* NotifyMulticastEventFilter_h */

