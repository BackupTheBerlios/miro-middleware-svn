// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
//  NotifyMulticast Config
//
//
//  (c) 2002, 2003
//  Department of Neural Information Processing, University of Ulm, Germany
//
//
//  Authors:
//    Philipp Baer <philipp.baer@informatik.uni-ulm.de>
//    Hans Utz <hans.utz@informatik.uni-ulm.de>
//
//
//  Version:
//    1.0.4
//
//
//  Description:
//
//    Holds all the configuration data for NotifyMulticast
//
//
//    Changes:
//
//      1.0.4
//      - added logging support
//
//      1.0.3
//      - many clean ups
//      - changed to own namespace
//      - renamed methods
//      - check for already subscribed MC group
//
//      1.0.2.pre
//      - initial release
//
//
//  $Id$
//
//////////////////////////////////////////////////////////////////////////////
#ifndef NotifyMulticastConfig_h
#define NotifyMulticastConfig_h

/* Miro includes */
#include "Exception.h"

/* ACE includes */
#include <ace/SOCK_Dgram_Mcast.h>
#include <ace/INET_Addr.h>
#include <ace/Date_Time.h>

/* TAO includes */
#include <orbsvcs/CosNotifyChannelAdminC.h>

/* g++lib includes */
#include <string>

namespace Miro 
{
  namespace NMC 
  {
    class Config 
    {
    public:
      //! Default constructor.
      Config();

      //! Default destructor.
      ~Config();

      /* Set/Get Eventchannel */
      void setEventchannel(CosNotifyChannelAdmin::EventChannel_ptr _eventChannel);
      CosNotifyChannelAdmin::EventChannel_ptr getEventchannel();

      /* Set/Get socket and get its address */
      void setSocket(ACE_INET_Addr &_address) throw(Miro::Exception);
      ACE_SOCK_Dgram_Mcast *getSocket();
      ACE_INET_Addr &getSocketAddress();
	  
      /* Set/Get domain */
      void setDomain(const std::string _domain);
      const std::string getDomain();
	  
      /* Set/Get EventMaxAge */
      void setEventMaxAge(unsigned long int _eventMaxAge);
      unsigned long int getEventMaxAge();
	  
    protected:
      CosNotifyChannelAdmin::EventChannel_var  eventChannel_;
	  
      ACE_INET_Addr                            socketAddress_;
      ACE_SOCK_Dgram_Mcast                    *socket_;
	  
      std::string                              domain_;
	  
      unsigned long int                        eventMaxAge_;
	  
      bool                                     subscribed_;
    };
  }
}
#endif // NotifyMulticastConfig_h

