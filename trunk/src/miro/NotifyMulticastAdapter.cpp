// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast Adapter
//
//
//  (c) 2002, 2003, 2004
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <phbaer@npw.net>
//    Hans Utz <hans.utz@informatik.uni-ulm.de>
//
//
//  Version:
//    1.1.0
//
//
//  Todo:
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////

/* NotifyMulticast includes */
#include "NotifyMulticastDefines.h"
#include "NotifyMulticastAdapter.h"
#include "NotifyMulticastSender.h"
#include "NotifyMulticastReceiver.h"
#include "NotifyMulticastEventHandler.h"
#include "NotifyMulticastParameters.h"
#include "NotifyMulticastSH.h"
/* ACE includes */
#include <ace/Arg_Shifter.h>
#include <ace/Thread.h>

/* TAO includes */
#include <tao/ORB_Core.h>


#define DBG_CLASSNAME "NotifyMulticast::Adapter"

namespace Miro {

    namespace NotifyMulticast {

        /**
         * Adapter::Adapter()
         *
         *   Description:
         *     Default constructor
         *
         *   Parameters:
         *     _argc:             Argument count
         *     _argv:             Argument vector
         *     _client:           Pointer to Miro client
         *     _eventChannel:     Pointer to eventchannel
         *     _eventMaxAge:      Max. age of an event in milliseconds
         *     _timeoutInterval:  Periodial checks for event fragments that
         *                        were not processed (-> drop)
         *     _multicastAddress: Multicast address spec
         *
         *   Throws:
         *     CORBA::Exception
         *     Miro::Exception
         */
        Adapter::Adapter(
                int _argc,
                char *_argv[],
                Miro::Client *_client,
                CosNotifyChannelAdmin::EventChannel_ptr _eventChannel,
                unsigned int _eventMaxAge,
                std::string _multicastAddress)
            throw(CORBA::Exception, Miro::Exception) :
                client_(_client),
                reactor_(_client->orb()->orb_core()->reactor()),
                configuration_(),
                eventHandlerId_(-1),
                eventHandler_(NULL),
                timeoutHandlerId_(-1),
                timeoutHandler_(NULL),
                timeoutHandlerInterval_(ACE_Time_Value(0, 50000)),
                shInterval_(ACE_Time_Value(2, 0)),
                useLogfile_(false)
        {
            PRINT_DBG(DBG_INFO, "Initializing");

            Parameters *parameters = Parameters::instance();

            // remove!
            _eventMaxAge = 0;
            _multicastAddress = "";

            /* set up configuration */
            configuration_.setSocket(parameters->multicastgroup);
            configuration_.setDomain(_client->namingContextName);
            configuration_.setEventchannel(_eventChannel);
            configuration_.setEventMaxAge(parameters->messagetimeout);

            /* process parameters */
            ACE_Arg_Shifter arg_shifter(_argc, _argv);

            while (arg_shifter.is_anything_left()) {
                if (arg_shifter.is_option_next()) {
                    if (!arg_shifter.cur_arg_strncasecmp("-mcastgroup")) {
                        std::string temp (arg_shifter.get_the_parameter("-mcastgroup"));
                        ACE_INET_Addr iaddr((temp + ":41006").c_str());
                        configuration_.setSocket(iaddr);
                        arg_shifter.consume_arg();
                    } else {
                        arg_shifter.ignore_arg();
                    }
                } else {
                    arg_shifter.ignore_arg();
                }
            }

            /* Setup sender and receiver */
            sender_ = new Sender(this, &configuration_);

            receiver_ = new Receiver(this, &configuration_);

            /* Create handlers */
            timeoutHandler_ = new TimeoutHandler(receiver_);

            eventHandler_   = new EventHandler(receiver_, &configuration_);

            sh_ = new SH(sender_, receiver_, &configuration_);

            receiver_->setSH(sh_);

            /* Install handlers */
            if (reactor_ != 0) {
		    std::cout << "Socket: " << configuration_.getSocket()->get_handle() << std::endl;
                eventHandlerId_ = reactor_->register_handler(
                                      configuration_.getSocket()->get_handle(),
                                      eventHandler_,
                                      ACE_Event_Handler::READ_MASK);

                if (eventHandlerId_ == -1) {
                    PRINT("Cannot register event handler");
                    throw Exception("Cannot register event handler");
                }

                timeoutHandlerId_ = reactor_->schedule_timer(
                                        timeoutHandler_,
                                        0,
                                        ACE_Time_Value(0,0),
                                        timeoutHandlerInterval_);

                shId_ = reactor_->schedule_timer(
                           sh_,
                           0,
                           ACE_Time_Value(0, 0),
                           shInterval_);

                if (timeoutHandlerId_ == -1) {
                    throw Exception("Cannot schedule timeout handler");
                }

            } else {
                throw Exception("Reactor not available");
            }

            char group[255];
            parameters->multicastgroup.addr_to_string(group, sizeof(group));

            PRINT("==================================");
            PRINT(" Initialized Multicast Forwarding");
            PRINT(" Multicast Group: " << group);
            PRINT("==================================");

            /* log details */
            LOG(&configuration_, "NotifyMulticast successfully initialized:");
            LOG(&configuration_, "  EventHandler (reactor):   " << eventHandlerId_);
            LOG(&configuration_, "  TimeoutHandler (reactor): " << timeoutHandlerId_);
        }


        /**
         * Adapter::~Adapter()
         *
         *   Description:
         *     Default destructor
         *
         *   Throws:
         *     CORBA::Exception
         *     Miro::Exception
         */
        Adapter::~Adapter() throw(CORBA::Exception, Exception) {
            PRINT_DBG(DBG_INFO, "Cleaning up");

	    try {
                if (timeoutHandlerId_ != -1) {
                    reactor_->cancel_timer(timeoutHandlerId_);
		}
            } catch(...) {
		PRINT("Error cancelling timeout timer");
	    }

	    try {
                if (shId_ != -1) {
                    reactor_->cancel_timer(shId_);
		}
            } catch(...) {
		PRINT("Error cancelling sh timer");
	    }

	    try {
	        if (eventHandlerId_ != -1) {
		    reactor_->remove_handler(eventHandler_, ACE_Event_Handler::READ_MASK);
		}
	    } catch(...) {
		PRINT("Error removing event handler");
	    }

	    PRINT("Deleting handlers (1/2)...");
            delete timeoutHandler_;
	    PRINT("+ timeout handler");
            delete eventHandler_;
	    PRINT("+ event handler");

	    PRINT("Invalidating SH...");
	    receiver_->invalidateSH();
	    
	    PRINT("Deleting handlers (2/2)...");
            delete sh_;
	    PRINT("+ sh");
            delete receiver_;
	    PRINT("+ receiver");
            delete sender_;
	    PRINT("+ sender");

            LOG(&configuration_, "NotifyMulticast successfully terminated");

            PRINT_DBG(DBG_MORE, "Cleaned up");
        }


        void Adapter::init() {
            PRINT_DBG(DBG_MORE, "Initializing subscriptions");
            sender_->init();
        }

        void Adapter::fini() {
            PRINT_DBG(DBG_MORE, "Finalizing notify multicast adapter.");
            receiver_->disconnect();
        }


        /**
         * Adapter::getSender()
         *
         *   Description:
         *     Returns pointer to sender object
         */
        Sender *Adapter::getSender() {
            return sender_;
        }


        /**
         * Adapter::getReceiver()
         *
         *   Description:
         *     Returns pointer to receiver object
         */
        Receiver *Adapter::getReceiver() {
            return receiver_;
        }


    };

};

