// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
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
  ChannelManager(int& argc, char * argv[], bool _shared, bool _unified, 
		 QString const& _channelName);
  ~ChannelManager();

  CosNotifyChannelAdmin::EventChannel_ptr getEventChannel(QString const& _domainName);
  bool debugLocalize() const;
  void setDebugLoclaize(bool _debug);

protected:
  typedef std::map<QString, CosNotifyChannelAdmin::EventChannel_ptr> ChannelMap;

  bool shared_;
  bool unified_;
  bool debugLocalize_;

  QString channelName_;

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
