// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast RequestEntry
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
//  Todo:
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

/* NotifyMulticast includes */
#include "NotifyMulticastRequestEntry.h"

#include <iostream>

#define DBG_CLASSNAME "NotifyMulticast::RequestEntry"

namespace Miro {

    namespace NotifyMulticast {

        /**
         * RequestEntry::RequestEntry()
         *
         *   Descritpion:
         *     Default destructor
         */
        RequestEntry::~RequestEntry() {
            if (ownReceivedFragments_) {
                ownReceivedFragments_ = 0;
                delete[] receivedFragments_;
            }
        }


        /**
         * RequestEntry::RequestEntry()
         *
         *   Descritpion:
         *     Default constructor
         *
         *   Parameters:
         *     byteOrder:     byte order
         *     requestId:     request id
         *     requestSize:   request size
         *     fragmentCount: fragment count
         */
        RequestEntry::RequestEntry(CORBA::Boolean byteOrder,
                                   CORBA::ULong   requestId,
                                   CORBA::ULong   requestSize,
                                   CORBA::ULong   fragmentCount) :
                byteOrder_(byteOrder),
                requestId_(requestId),
                requestSize_(requestSize),
                fragmentCount_(fragmentCount),
        timeoutCounter_(0) {
            const int bitsPerUlong = sizeof(CORBA::ULong) * CHAR_BIT;

            ACE_CDR::grow(&payload_, requestSize_);
            payload_.wr_ptr(requestSize_);

            receivedFragments_ = defaultReceivedFragments_;
            ownReceivedFragments_ = 0;

            receivedFragmentsSize_ = fragmentCount_ / bitsPerUlong + 1;

            if (receivedFragmentsSize_ > DEFAULT_FRAGMENT_BUFSIZ) {
                ACE_NEW(receivedFragments_, CORBA::ULong[receivedFragmentsSize_]);
                ownReceivedFragments_ = 1;
            }

            for (CORBA::ULong i = 0; i < receivedFragmentsSize_; ++i)
                receivedFragments_[i] = 0;

            CORBA::ULong idx = fragmentCount_ / bitsPerUlong;

            CORBA::ULong bit = fragmentCount_ % bitsPerUlong;

            receivedFragments_[idx] = (0xFFFFFFFF << bit);
        }


        /**
         * RequestEntry::validateFragment()
         *
         *   Description:
         *     Validates a given fragment
         *
         *   Parameters:
         *     byteOrder:      byte order
         *     requestSize:    request size
         *     fragmentSize:   fragment size
         *     fragmentOffset: fragment offset
         *     fragmentId:     fragment id
         *     fragmentCount:  fragment count
         */
        int RequestEntry::validateFragment(CORBA::Boolean byteOrder,
                                           CORBA::ULong   requestSize,
                                           CORBA::ULong   fragmentSize,
                                           CORBA::ULong   fragmentOffset,
                                           CORBA::ULong   /* fragmentId */,
                                           CORBA::ULong   fragmentCount) const {

            if (byteOrder     != byteOrder_ ||
                    requestSize   != requestSize_ ||
                    fragmentCount != fragmentCount_)
                return 0;

            if (fragmentOffset                >= requestSize ||
                    fragmentOffset + fragmentSize >  requestSize)
                return 0;

            return 1;
        }


        /**
         * RequestEntry::testReceived()
         *
         *   Descritpion:
         *     Tests, if eevent already received
         *
         *   Parameters:
         *     fragmentId: fragment id
         */
        int RequestEntry::testReceived(CORBA::ULong fragmentId) const {
            const int bitsPerUlong = sizeof(CORBA::ULong) * CHAR_BIT;

            // Assume out-of-range fragments as received, so they are dropped...

            if (fragmentId > fragmentCount_)
                return 1;

            CORBA::ULong idx = fragmentId / bitsPerUlong;

            CORBA::ULong bit = fragmentId % bitsPerUlong;

            return ACE_BIT_ENABLED(receivedFragments_[idx], 1 << bit);
        }


        /**
         * RequestEntry::markReceived()
         *
         *   Descritpion:
         *     Marks received fragments
         *
         *   Parameters:
         *     fragmentId: fragment id
         */
        void RequestEntry::markReceived(CORBA::ULong fragmentId) {
            const int bitsPerUlong = sizeof(CORBA::ULong) * CHAR_BIT;

            // Assume out-of-range fragments as received, so they are dropped...

            if (fragmentId > fragmentCount_)
                return;

            CORBA::ULong idx = fragmentId / bitsPerUlong;

            CORBA::ULong bit = fragmentId % bitsPerUlong;

            ACE_SET_BITS(receivedFragments_[idx], 1 << bit);
        }


        /**
         * RequestEntry::complete()
         *
         *   Description:
         *     Tests, is entry was received completely
         */
        int RequestEntry::complete (void) const {
            for (CORBA::ULong i = 0; i < receivedFragmentsSize_; ++i)
                if (receivedFragments_[i] != 0xFFFFFFFF)
                    return 0;

            return 1;
        }


        /**
         * RequestEntry::fragment_buffer()
         *
         *   Descritpion:
         *     Returns a pointer to the fragment buffer plus fragmentOffset
         *
         *   Parameters:
         *     fragmentOffset: fragment offset
         */
        char* RequestEntry::fragmentBuffer(CORBA::ULong fragmentOffset) {
            return payload_.rd_ptr() + fragmentOffset;
        }


        /**
         * RequestEntry::decode()
         *
         *   Description:
         *     Decodes (unmarshals) the request entry into an event
         *
         *   Parameters:
         *     event: CosNotification::StructuredEvent object
         */
        void RequestEntry::decode(CosNotification::StructuredEvent &event
                                  ACE_ENV_ARG_DECL) {
            CosNotification::EventHeader          header;
            CosNotification::FilterableEventBody  filterableBody;
            CORBA::Any                            remainder;

            TAO_InputCDR cdr(&payload_, ACE_static_cast(int, byteOrder_));

            if (!(cdr >> event)) {
                ACE_THROW(CORBA::MARSHAL());
            }
        }


        /**
         * RequestEntry::incTimeout()
         *
         *   Descritpion:
         *     Increments the timeout counter
         */
        void RequestEntry::incTimeout(void) {
            timeoutCounter_++;
        }


        /**
         * RequestEntry::getTimeout()
         *
         *   Descritpion:
         *     Returns the timeout
         */
        int RequestEntry::getTimeout(void) const {
            return timeoutCounter_;
        }
    }
}
