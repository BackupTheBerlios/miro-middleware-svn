#// -*- c++ -*- ///////////////////////////////////////////////////////////////
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
//  Description:
//
//    This event handler periodically calls (with the help of a reactor) the
//    handle_timeout routine of the receiver to invalidate events that are
//    incomplete for longer time.
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef NotifyMulticastTimeoutHandler_h
#define NotifyMulticastTimeoutHandler_h

#include <ace/Event_Handler.h>


namespace Miro {

    namespace NotifyMulticast {

        class Receiver;

        class TimeoutHandler : public ACE_Event_Handler {
                typedef ACE_Event_Handler Super;

            public:
                TimeoutHandler(Receiver *_receiver);

                virtual int handle_timeout(const ACE_Time_Value &_tv,
                                           const void           *_act);

            protected:
                Receiver *receiver_;

        };
    };
};

#endif
