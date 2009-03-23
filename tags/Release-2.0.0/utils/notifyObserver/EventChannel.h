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
#ifndef notifyObserver_EventChannel_h
#define notifyObserver_EventChannel_h

#include "utils/widgets/Item.h"


#include <orbsvcs/CosNotifyChannelAdminS.h>

#include <vector>

class EventChannel : public Item
{
  Q_OBJECT
  
  typedef Item Super;

public:
  EventChannel(CosNotifyChannelAdmin::EventChannel_ptr _ec,
	       CosNotifyChannelAdmin::AdminID _id,
	       QListViewItem * _parentItem,
	       QObject * _parent = 0, const char * _name = 0);

  virtual void update();
  virtual void contextMenu(QPopupMenu& _menu);

  void setEventChannel(CosNotifyChannelAdmin::EventChannel_ptr);
  CosNotifyChannelAdmin::ConsumerAdmin_ptr 
  getConsumerAdmin(CosNotifyChannelAdmin::AdminID _id);
  CosNotifyChannelAdmin::SupplierAdmin_ptr
  getSupplierAdmin(CosNotifyChannelAdmin::AdminID _id);
  CosNotifyChannelAdmin::ChannelID id() const;
  
protected slots:
  void destroy();
  void createSupplierAdmin();
  void createConsumerAdmin();

protected:
  //! Event channel.
  CosNotifyChannelAdmin::EventChannel_var ec_;
  //! Event channel id at the factory
  CosNotifyChannelAdmin::ChannelID id_;
};

inline
CosNotifyChannelAdmin::ChannelID 
EventChannel::id() const {
  return id_;
}

inline
void
EventChannel::setEventChannel(CosNotifyChannelAdmin::EventChannel_ptr _ec) {
  ec_ = CosNotifyChannelAdmin::EventChannel::_duplicate(_ec);
}

#endif // notifyObserver_EventChannel_h
