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
#ifndef notifyObserver_EventChannelFactory_h
#define notifyObserver_EventChannelFactory_h

#include "utils/widgets/Item.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>

#include <vector>

// forward declarations
class QListView;

class EventChannelFactory : public Item
{
  Q_OBJECT
  
  typedef Item Super;

public:
  typedef std::vector<CosNotifyChannelAdmin::ChannelID> ChannelIDVector;

  EventChannelFactory(CosNotifyChannelAdmin::EventChannelFactory_ptr _notifyFactory,
		      QListView * _listView,
		      QObject * _parent = 0, const char * _name = 0);

  virtual void update();
  virtual void contextMenu(QPopupMenu& _menu);


public slots:
  void updateFactory();

protected slots:
  void createChannel();

protected:
  CosNotifyChannelAdmin::EventChannel_ptr getChannel(CosNotifyChannelAdmin::ChannelID _id);
  
  //! Channel factory.
  CosNotifyChannelAdmin::EventChannelFactory_var notifyFactory_;
};

#endif // notifyObserver_EventChannelFactory_h
