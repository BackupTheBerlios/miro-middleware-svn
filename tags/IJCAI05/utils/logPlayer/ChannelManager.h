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
  ChannelManager(int& argc, char * argv[], bool _shared, bool _unified);
  ~ChannelManager();

  CosNotifyChannelAdmin::EventChannel_ptr getEventChannel(QString const& _domainName);
  bool debugLocalize() const;
  void setDebugLoclaize(bool _debug);

protected:
  typedef std::map<QString, CosNotifyChannelAdmin::EventChannel_ptr> ChannelMap;

  bool shared_;
  bool unified_;
  bool debugLocalize_;

  CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory_;
  CosNotifyChannelAdmin::EventChannel_var unifiedChannel_;
  ChannelMap channel_;
};

inline
bool
ChannelManager::debugLocalize() const {
  return debugLocalize_;
}
inline
void
ChannelManager::setDebugLoclaize(bool _debug) {
  debugLocalize_ = _debug;
}

#endif // ChannelManager_h
