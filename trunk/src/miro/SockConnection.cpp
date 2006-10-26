/*!\file SockConnection.cpp
**CopyRight(c) 2005: Defence Research and Development Canada, Suffield
**
** Vehicle Concepts Group
** Autonomous Intelligent Systems
** Building 13a, Suffield, Alberta
**
** Title: SockConnection.cpp
**
** Author: G. Broten
**
** Based Upon: The original Ulm Miro TtyConnection.cpp
**
** Purpose:
** Define the Socket Connection class in a manner that is analogous to the Miro TtyConnection class.
** The constructor initializes the socket and registers the event_handler that handles the socket data.
**
** $Id:
**
*/

/* Standard Includes */
#include <iostream>

/* ACE Includes */
#include <ace/Reactor.h>
#include <ace/OS.h>

/* Miro Includes */
#include <miro/DevEventHandler.h>
#include <miro/Exception.h>
#include <miro/Log.h>

/* Local Includes */
#include "SvcParameters.h"
#include "SockConnection.h"

namespace Miro
{

  //
  // Constructors / Destructors
  //

	SockConnection::SockConnection(ACE_Reactor * _reactor,
		DevEventHandler * _eventHandler,
		const SockParameters& _parameters) :
	reactor_(_reactor),
	eventHandler_(_eventHandler),
	ifName_(_parameters.device.c_str()),
	hostName_(_parameters.host.c_str()),
	portName_(_parameters.port.c_str())
	{

		MIRO_DBG(MIRO, LL_NOTICE, "SockConnection initialising\n");

		/*
		**  Initialize and pointer to a socket as the connection method
		*/

		if (srvr_.set( portName_, hostName_ ) == -1 )
		{
			MIRO_LOG_OSTR(LL_CRITICAL,
				"Failed to set Address and Port: " << portName_ << hostName_);
			throw CException(errno, std::strerror(errno));
		}	

		if (connector_.connect(peer_, srvr_, 0, ACE_Addr::sap_any, 0, O_RDWR) == -1) 
		{
			MIRO_LOG_OSTR(LL_CRITICAL,
				"Failed to open device: " << hostName_ << ":" << portName_
				<< "\nEnsure the required Server is running?");
			throw CException(errno, std::strerror(errno));
		}

    	// we need a handler to listen to the asynchronous file handler
    	// of the device
		eventHandler_->set_handle(peer_.get_handle());
		int rc =
			reactor_->register_handler(eventHandler_, ::ACE_Event_Handler::READ_MASK);
		if (rc == -1)
			throw ACE_Exception(errno, "Failed to register handler for file descriptor.");
		MIRO_DBG(MIRO, LL_NOTICE, "SockConnection Complete\n");

	}

	SockConnection::~SockConnection()
	{
		MIRO_DBG(MIRO, LL_NOTICE, "Destructing SockConnection\n");

    	// Stop hardware triggered communication
		reactor_->remove_handler(eventHandler_,
			::ACE_Event_Handler::READ_MASK);
		peer_.close();
	}
	
}
