// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticastTimeoutHandler
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
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

/* NotifyMulticast includes */
#include "NotifyMulticastDefines.h"
#include "NotifyMulticastTimeoutHandler.h"
#include "NotifyMulticastReceiver.h"

#define DBG_CLASSNAME "NotifyMulticast::TimeoutHandler"

namespace Miro {

    namespace NotifyMulticast {

        /**
         * TimeoutHandler::TimeoutHandler()
         *
         *   Description:
         *     Timeout handler for periodical updates on fragment registry
         *
         *   Parameters:
         *     _receiver: Pointer to receiver instance
         */
        TimeoutHandler::TimeoutHandler(Receiver *_receiver) {
            PRINT_DBG(DBG_INFO, "Initializing");

            receiver_ = _receiver;

            PRINT_DBG(DBG_MORE, "Initialized");
        }


        /**
         * TimeoutHandler::handle_timeout()
         *
         *   Description:
         *     Called timer thread
         *
         *   Parameters:
         *     _tv:  time value for timeout
         *     _act: generic pointer (to whatever)
         */
        int TimeoutHandler::handle_timeout(const ACE_Time_Value &_tv,
                                           const void           *_act) {
            if (receiver_)
                this->receiver_->handle_timeout(_tv, _act);

            return 0;
        }
    };
};

