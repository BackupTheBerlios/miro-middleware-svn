// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast RequestIndex
//
//
//  (c) 2002
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <phbaer@openums.org>
//
//
//  Version:
//    1.0.0
//
//
//  Description:
//
//    ... to be written ...
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef NotifyMulticastRequestIndex_h
#define NotifyMulticastRequestIndex_h

#include <ace/INET_Addr.h>
#include <orbsvcs/CosNotifyCommC.h>

namespace Miro {

    namespace NotifyMulticast {

        class RequestIndex {

            public:
                /* Default constructor */
                RequestIndex(void) : requestId(0) {}

                ;

                /* Default copy constructor */
                RequestIndex(const ACE_INET_Addr &_from, CORBA::ULong _requestId) : from(_from), requestId(_requestId) {}

                ;

                /* return hash value */
                inline u_long hash (void) const {
                    return ((from.get_ip_address() << 24)
                            | (from.get_port_number () << 8)
                            | (requestId & 0x000000ff));
                }

                /* comparison operators */
                inline int operator== (const RequestIndex &rhs) const {
                    return (this->from == rhs.from &&
                            this->requestId == rhs.requestId);
                }

                inline int operator!= (const RequestIndex &rhs) const {
                    return !(*this == rhs);
                }

            private:
                ACE_INET_Addr from;
                CORBA::ULong  requestId;
        };
    };
};

#endif
