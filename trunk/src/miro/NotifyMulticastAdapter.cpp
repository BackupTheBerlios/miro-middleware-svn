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

#include "Log.h"

/* ACE includes */
#include <ace/Reactor.h>
#include <ace/SOCK_Dgram_Mcast.h>
#include <ace/INET_Addr.h>
#include <ace/Arg_Shifter.h>

/* TAO includes */
#include <tao/ORB_Core.h>

namespace Miro 
{
  namespace NMC 
  {
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
     *
     *   Throws:
     *     CORBA::Exception
     *     Miro::Exception
     */
    Adapter::Adapter(int& _argc,
		     char *_argv[],
		     Miro::Client * _client,
		     CosNotifyChannelAdmin::EventChannel_ptr _eventChannel,
		     std::string& _domainName,
		     Parameters * _parameters)
      throw(CORBA::Exception, Miro::Exception) :
      parameters_(_parameters),
      reactor_(_client->orb()->orb_core()->reactor()),
      socket_(new ACE_SOCK_Dgram_Mcast()),
      eventHandlerId_(-1),
      eventHandler_(NULL),
      timeoutHandlerId_(-1),
      timeoutHandler_(NULL),
      timeoutHandlerInterval_(ACE_Time_Value(0, 50000)),
      shInterval_(ACE_Time_Value(2, 0))
    {
      MIRO_LOG_CTOR("NMC::Adapter");

      /* process parameters */
      ACE_Arg_Shifter arg_shifter(_argc, _argv);

      while (arg_shifter.is_anything_left()) {
	if (arg_shifter.is_option_next()) {
	  if (!arg_shifter.cur_arg_strncasecmp("-MiroNofitfyMulticastGroup")) {
	    std::string addr (arg_shifter.get_the_parameter("-MiroNotifyMulticastGroup"));
	    parameters_->multicastGroup.set(addr.c_str());
	    arg_shifter.consume_arg();
	  } 
	  else if (!arg_shifter.cur_arg_strncasecmp("-MNMG")) {
	    std::string addr (arg_shifter.get_the_parameter("-MNMG"));
	    parameters_->multicastGroup.set(addr.c_str());
	    arg_shifter.consume_arg();
	  } 
	  else {
	    arg_shifter.ignore_arg();
	  }
	} 
	else {
	  arg_shifter.ignore_arg();
	}
      }

      // Set up connection socket
      if (socket_->subscribe(parameters_->multicastGroup) == -1) {
	throw Miro::Exception("Cannot subscribe multicast address");
      }

      /* Setup sender and receiver */
      sender_ = new Sender(*socket_, _eventChannel, _domainName, parameters_);
      receiver_ = new Receiver(*socket_, _eventChannel, _domainName, parameters_);

      /* Create handlers */
      timeoutHandler_ = new TimeoutHandler(receiver_);
      eventHandler_   = new EventHandler(socket_->get_handle(), receiver_);
      sh_ = new SH(sender_, receiver_);

      receiver_->setSH(sh_);

      /* Install handlers */
      if (reactor_ != 0) {
	eventHandlerId_ = reactor_->register_handler(eventHandler_,
						     ACE_Event_Handler::READ_MASK);

	if (eventHandlerId_ == -1) {
	  throw Exception("Cannot register event handler");
	}

	timeoutHandlerId_ = reactor_->schedule_timer(timeoutHandler_,
						     0,
						     ACE_Time_Value(0,0),
						     timeoutHandlerInterval_);

	shId_ = reactor_->schedule_timer(sh_,
					 0,
					 ACE_Time_Value(0, 0),
					 shInterval_);

	if (timeoutHandlerId_ == -1) {
	  throw Exception("Cannot schedule timeout handler");
	}

      } 
      else {
	throw Exception("Reactor not available");
      }

      char group[255];
      parameters_->multicastGroup.addr_to_string(group, sizeof(group));

      MIRO_LOG_OSTR(LL_NOTICE, 
		    " Initialized Multicast Forwarding. Multicast Group: " << group);

      /* log details */
      MIRO_DBG_OSTR(NMC, LL_DEBUG,
		    "NotifyMulticast successfully initialized:" << std::endl <<
		    "  EventHandler (reactor):   " << eventHandlerId_ << std::endl <<
		    "  TimeoutHandler (reactor): " << timeoutHandlerId_);
    }

    Adapter::~Adapter() throw() 
    {
      MIRO_LOG_DTOR("NMC::Adapter");

      if (timeoutHandlerId_ != -1)
	reactor_->cancel_timer(timeoutHandlerId_);
      if (shId_ != -1) 
	reactor_->cancel_timer(shId_);
      if (eventHandlerId_ != -1) 
	reactor_->remove_handler(eventHandler_, ACE_Event_Handler::READ_MASK);

      MIRO_DBG(NMC, LL_DEBUG, "Invalidating SH.");
      receiver_->invalidateSH();
      MIRO_DBG(NMC, LL_DEBUG, "Unsubscribing from multicast group.");
      socket_->unsubscribe(parameters_->multicastGroup);

      MIRO_DBG(NMC, LL_DEBUG, "Deleting timeout handler.");
      delete timeoutHandler_;
      MIRO_DBG(NMC, LL_DEBUG, "Deleting event handler.");
      delete eventHandler_;
	    
      MIRO_DBG(NMC, LL_DEBUG, "Deleting sh.");
      delete sh_;
      MIRO_DBG(NMC, LL_DEBUG, "Deleting receiver.");
      delete receiver_;
      MIRO_DBG(NMC, LL_DEBUG, "Deleting sender.");
      delete sender_;
      MIRO_DBG(NMC, LL_DEBUG, "Deleting socket.");
      delete socket_;

      MIRO_LOG(LL_NOTICE, "NotifyMulticast successfully terminated.");
    }


    void Adapter::init() {
      MIRO_DBG(NMC, LL_DEBUG, "Initializing subscriptions");
      sender_->init();
    }

    void Adapter::fini() {
      MIRO_DBG(NMC, LL_DEBUG, "Finalizing notify multicast adapter.");
      sender_->disconnect();
      receiver_->disconnect();
    }
  } // NMC
} // MIRO

