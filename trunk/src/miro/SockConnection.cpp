// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 2005 Defence Research and Development Canada, Suffield
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
// Authors: G. Broten
//

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
      host(_parameters.host.c_str()),
      port(atoi( _parameters.port.c_str() )),
      sock_type( _parameters.type )
  {
	  /* Variables */

     MIRO_DBG(MIRO, LL_NOTICE, "SockConnection Initialization\n");   

   /*
    ** How the connection is made depends on the requested socket type
    */

    switch( sock_type )
    {
		/* TCP Socket */
       	case TCP:
		 /*
		 **  Initialize and pointer to a socket as the connection method
		 */

		 if (srvr.set(port, host) == -1) {
			  MIRO_LOG_OSTR(LL_CRITICAL,
						"Failed to set Address and Port: " << port << ":" << host << "\n");
			  throw;
		   }

 		   /*
		   ** Is this a stream (TCP socket) or a user datagram (UDP socket)
		   */ 
		   if (connector_.connect(peer_tcp_, srvr, 0, ACE_Addr::sap_any, 0, O_RDWR) == -1) 
		   {
			  MIRO_LOG_OSTR(LL_CRITICAL,
					"TCP Failed to open device: " << host << ":" << port
					<< "\nEnsure the required Server is running?");
			  throw;
		   }
		   
		   // we need a handler to listen to the asynchronous file handler
		   // of the device
		   eventHandler_->set_handle(peer_tcp_.get_handle());
           break;

		/* UDP Socket */
       	case UDP:
          srvr.set_port_number( port );
           if (peer_udp_.open(srvr,ACE_PROTOCOL_FAMILY_INET,0,1) == -1)
           {
              MIRO_LOG_OSTR(LL_CRITICAL,
                "UDP Failed to open device: " << host << ":" << port );
              throw;
           }
           MIRO_LOG_OSTR(LL_CRITICAL, "UDP SockConnection Port Number " << port << "\n");

     	   // we need a handler to listen to the asynchronous file handler
    	   // of the device
      	   eventHandler_->set_handle(peer_udp_.get_handle());
           break;

		/* Broadcast */
       	case BROADCAST:
          break;

		/* Multicast */
       	case MULTICAST:
		 /*
		 **  Initialize and pointer to a socket as the connection method
		 */

		 if (srvr.set(port, host) == -1) {
			  MIRO_LOG_OSTR(LL_CRITICAL,
						"Failed to set Address and Port: " << port << ":" << host << "\n");
			  throw;
		   }

			if (multicast_dgram.join( srvr ) == -1 )
			{
				 MIRO_LOG_OSTR(LL_CRITICAL,
				   "Multicast Failed to join device: " << host << ":" << port << "\n");
				  throw;
			}
						
			// we need a handler to listen to the asynchronous file handler
			// of the device
			eventHandler_->set_handle(multicast_dgram.get_handle());
			
           break;

		default:
            MIRO_LOG_OSTR(LL_CRITICAL, "Invalid Socket Type" << sock_type  );
     }

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

    switch( sock_type)
    {
		/* TCP Socket */
       	case TCP:
      	   peer_tcp_.close();
		break;
 
		/* UDP Socket */
       	case UDP:
           peer_udp_.close();
		break;
	
		/* Broadcast */
       	case BROADCAST:
		break;

		/* Multicast */
       	case MULTICAST:
           multicast_dgram.leave(srvr);
		break;

		default:
            MIRO_LOG_OSTR(LL_CRITICAL, "Invalid Socket Type" << sock_type  );

   }
  }

   /*
   ** Generic method for sending/receiving data via either tcp or udp
   */
   int SockConnection::socksend( unsigned char *buf, int size )
   {
     int status;

    switch( sock_type)
    {
	/* TCP Socket */
       	case TCP:
           status = peer_tcp_.send_n(buf, size);
	   break;
 
	/* UDP Socket */
       	case UDP:
           status = peer_tcp_.send_n(buf, size);
	   break;
	
	/* Broadcast */
       	case BROADCAST:
	   break;

	/* Multicast */
       	case MULTICAST:
           status = multicast_dgram.send(buf, size);
	   break;

		default:
            MIRO_LOG_OSTR(LL_CRITICAL, "Invalid Socket Type" << sock_type  );
   }
     return(status);
   }
   int SockConnection::sockrecv( unsigned char *buf, int size )
   {
    /* Variable */
	int status;
    ACE_INET_Addr remote_address((u_short)0);

    switch( sock_type )
    {
		/* TCP Socket */
       	case TCP:
           status = peer_tcp_.recv(buf, size);
	   break;
 
		/* UDP Socket */
       	case UDP:
           status = peer_tcp_.recv(buf, size);
	   break;
	
		/* Broadcast */
       	case BROADCAST:
	   break;

		/* Multicast */
       	case MULTICAST:
           status = multicast_dgram.recv(buf, size, remote_address);
           std::cout <<"Mutlicast bytes: " << status << std::endl;
	   break;

		default:
            MIRO_LOG_OSTR(LL_CRITICAL, "Invalid Socket Type" << sock_type  );

   }
   return(status);
  }
}
