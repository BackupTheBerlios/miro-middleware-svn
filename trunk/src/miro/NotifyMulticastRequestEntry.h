// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticastRequestEntry
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
//    1.0.3
//
//
//  Description:
//    Container for events received via MC/EC
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef NotifyMulticastRequestEntry_h
#define NotifyMulticastRequestEntry_h

/* NotifyMulticast includes */
#include "NotifyMulticastRequestIndex.h"

/* ACE includes */
#include <ace/Message_Block.h>
#include <ace/SOCK_Dgram_Mcast.h>

/* TAO includes */
#include <orbsvcs/CosNotifyChannelAdminS.h>
#include <orbsvcs/CosNotifyCommC.h>

// Hack for TAO <= x.2.1 compability

#ifndef ACE_ENV_ARG_DECL_WITH_DEFAULTS
#define ACE_ENV_ARG_DECL_WITH_DEFAULTS \
 , CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ()
#endif
#ifndef ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
#define ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS \
 CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ()
#endif
#ifndef ACE_ENV_ARG_DECL
#define ACE_ENV_ARG_DECL \
 , CORBA::Environment & _ACE_CORBA_Environment_variable
#endif
#ifndef ACE_ENV_SINGLE_ARG_DECL
#define ACE_ENV_SINGLE_ARG_DECL \
 CORBA::Environment & _ACE_CORBA_Environment_variable
#endif
#ifndef ACE_ENV_ARG_DECL_NOT_USED
#define ACE_ENV_ARG_DECL_NOT_USED \
 , CORBA::Environment &
#endif
#ifndef ACE_ENV_SINGLE_ARG_DECL_NOT_USED
#define ACE_ENV_SINGLE_ARG_DECL_NOT_USED \
 CORBA::Environment &
#endif

namespace Miro {

    namespace NotifyMulticast {

        class RequestEntry {

            public:
                enum {
                    DEFAULT_FRAGMENT_BUFSIZ = 8
            };

                ~RequestEntry (void);

                /* Initialize the fragment, allocating memory, etc. */
                RequestEntry(CORBA::Boolean byteOrder,
                             CORBA::ULong   requestId,
                             CORBA::ULong   requestSize,
                             CORBA::ULong   fragmentCount);

                /* Validate a fragment, it should be rejected if it is invalid.. */
                int validateFragment(CORBA::Boolean byteOrder,
                                     CORBA::ULong   requestSize,
                                     CORBA::ULong   fragmentSize,
                                     CORBA::ULong   fragmentOffset,
                                     CORBA::ULong   fragmentId,
                                     CORBA::ULong   fragmentCount) const;

                /* Has <fragment_id> been received? */
                int testReceived(CORBA::ULong fragmentId) const;

                /* Mark <fragment_id> as received, reset timeout counter... */
                void markReceived(CORBA::ULong fragmentId);

                /* Is the message complete? */
                int complete(void) const;

                /* Return a buffer for the fragment at offset <fragment_offset> */
                char* fragmentBuffer(CORBA::ULong fragmentOffset);

                /* Decode the events, the message must be complete. */
                void decode(CosNotification::StructuredEvent &event
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS);

                /* Increment the timeout counter... */
                void incTimeout(void);

                /* Get the timeout counter.... */
                int getTimeout(void) const;

            private:
                /* This attributes should remain constant in all the fragments, used *
                 * for validation....                                                */
                CORBA::Boolean    byteOrder_;
                CORBA::ULong      requestId_;
                CORBA::ULong      requestSize_;
                CORBA::ULong      fragmentCount_;

                CORBA::ULong      timeoutCounter_;
                ACE_Message_Block payload_;

                /* This is a bit vector, used to keep track of the received buffers. */
                CORBA::ULong     *receivedFragments_;
                int               ownReceivedFragments_;
                CORBA::ULong      receivedFragmentsSize_;
                CORBA::ULong      defaultReceivedFragments_[DEFAULT_FRAGMENT_BUFSIZ];
        };
    };
};

#endif
