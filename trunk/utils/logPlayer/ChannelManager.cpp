// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2003
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ChannelManager.h"

#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::cin;

ChannelManager::ChannelManager(int& argc, char * argv[], 
			       bool _shared, bool _unified) :
  Super(argc, argv),
  shared_(_shared),
  unified_(_unified)
{
  if (!shared_) {
    // Channel factory
    notifyFactory_ =
      TAO_Notify_EventChannelFactory_i::create(poa.in() ACE_ENV_ARG_PARAMETER);
    
    addToNameService(notifyFactory_.in(), initialNamingContext.in(), 
		     "NotifyEventChannelFactory");
  }
  if (unified_) {
    // Channel identifer.
    CosNotifyChannelAdmin::ChannelID id;
    // Initial qos specified to the factory when creating the EC.
    CosNotification::QoSProperties initialQos;
    // Initial admin props specified to the factory when creating the EC.
    CosNotification::AdminProperties initialAdmin;

    unifiedChannel_ = notifyFactory_->create_channel(initialQos, initialAdmin, id);
  }
}

ChannelManager::~ChannelManager()
{
  if (!shared_) {
    CosNaming::Name n;
    n.length(2);
    n[1].id = CORBA::string_dup("EventChannel");

    ChannelMap::iterator first, last = channel_.end();
    for (first = channel_.begin(); first != last; ++first) {
      n[0].id = CORBA::string_dup(first->first.latin1());
      initialNamingContext->unbind(n);
    }
  }
}

CosNotifyChannelAdmin::EventChannel_ptr 
ChannelManager::getEventChannel(QString const& _domainName)
{
  ChannelMap::iterator channel = channel_.find(_domainName);

  if (channel == channel_.end()) {

    // The channel reference we need to get - somehow  
    CosNotifyChannelAdmin::EventChannel_ptr ec;
    CosNaming::Name n;
    n.length(2);
    n[0].id = CORBA::string_dup(_domainName.latin1());
    n[1].id = CORBA::string_dup("EventChannel");

    if (shared_) {
      ec = resolveName<CosNotifyChannelAdmin::EventChannel>(n);
    }
    else {
      if (rebind_) {
	// Force binding of references to make
	// sure they are always up-to-date.
	try {
	  initialNamingContext->unbind(n);
	  
	  std::cerr << "EventChannel was still bound in naming service." 
		    << "Unbound it." << endl;
	} catch (...) {
	  // catch anything in case there wasn't anything bound
	}
      }

      // Channel identifer.
      CosNotifyChannelAdmin::ChannelID id;
      // Initial qos specified to the factory when creating the EC.
      CosNotification::QoSProperties initialQos;
      // Initial admin props specified to the factory when creating the EC.
      CosNotification::AdminProperties initialAdmin;

      if (!unified_) {
	ec = notifyFactory_->create_channel(initialQos, initialAdmin, id);
      }
      else {
	ec = unifiedChannel_.in();
      }

      // Attempt to create naming context.
      CosNaming::NamingContext_var namingContext;
      n.length(1);
      n[0].id = CORBA::string_dup(_domainName.latin1());
      try {
	namingContext = initialNamingContext->bind_new_context(n);
      } 
      catch (CosNaming::NamingContext::AlreadyBound const&) {
	// Fine, CCS context already exists.
	// get a reference for it
	namingContext = resolveName<CosNaming::NamingContext>(n);
      }

      n.length(1);
      n[0].id = CORBA::string_dup("EventChannel");
      namingContext->bind(n, ec);
    }
    channel = channel_.insert(std::make_pair(_domainName,
					     CosNotifyChannelAdmin::EventChannel::_duplicate(ec))).first; 
  }

  return channel->second;
}
