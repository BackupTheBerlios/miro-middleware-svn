// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast EventHandler
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

#include "NotifyMulticastDefines.h"
#include "NotifyMulticastEventHandler.h"
#include "NotifyMulticastReceiver.h"
#include "NotifyMulticastConfig.h"

#define DBG_CLASSNAME "NotifyMulticast::EventHandler"

namespace Miro {

    namespace NotifyMulticast {

        /**
         * EventHandler::EventHandler()
         *
         *   Description:
         *     Default constructor
         *
         *   Parameters:
         *     _receiver: Pointer to receiver object
         *     _config:   Pointer to NotifyMulticast configuration
         */
        EventHandler::EventHandler(Receiver *_receiver, Config *_config) {
            PRINT_DBG(DBG_INFO, "Initializing");

            receiver_ = _receiver;
            config_ = _config;

            PRINT_DBG(DBG_MORE, "Initialized");
        }

        /**
         * EventHandler::handle_input()
         *
         *   Description:
         *     Is called when input is available
         */
        int EventHandler::handle_input(ACE_HANDLE /*handle*/) {
            this->receiver_->handle_input();
            return 0;
        }


        /**
         * EventHandler::get_handle()
         *
         *   Description:
         *     Returns ACE handle
         */
        ACE_HANDLE EventHandler::get_handle() const {
            return config_->getSocket()->get_handle();;
        }
    };
};

