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
#ifndef ChannelManager_h
#define ChannelManager_h

#include "miro/Server.h"

#include <orbsvcs/CosNotifyChannelAdminC.h>

#include <qstring.h>

#include <map>

class ChannelManager : public Miro::Server
{
  typedef Miro::Server Super;

public:
  ChannelManager(int& argc, char * argv[], bool _shared);
  ~ChannelManager();

  CosNotifyChannelAdmin::EventChannel_ptr getEventChannel(QString const& _domainName);

protected:
  typedef std::map<QString, CosNotifyChannelAdmin::EventChannel_ptr> ChannelMap;

  bool shared_;
  CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory_;
  ChannelMap channel_;
};

#endif // ChannelManager_h
