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
#ifndef notifyObserver_ConsumerAdmin_h
#define notifyObserver_ConsumerAdmin_h

#include "utils/widgets/Item.h"


#include <orbsvcs/CosNotifyChannelAdminS.h>

// forward declaration
class NotifySubscribe;

class ConsumerAdmin : public Item
{
  Q_OBJECT
  
  typedef Item Super;

public:
  ConsumerAdmin(CosNotifyChannelAdmin::ConsumerAdmin_ptr _ca,
	       QListViewItem * _parentItem,
	       QObject * _parent = 0, const char * _name = 0);

  virtual void contextMenu(QPopupMenu& _menu);

  void setConsumerAdmin(CosNotifyChannelAdmin::ConsumerAdmin_ptr);
  CosNotifyChannelAdmin::ProxySupplier_ptr
  getProxyPushSupplier(CosNotifyChannelAdmin::ProxyID _id);
  CosNotifyChannelAdmin::AdminID id() const;

signals:
  void updateAdmin();

protected slots:
  void destroy();
  void createProxyPushSupplier();

protected:
  //! NotifySubscribe interface.
  NotifySubscribe * notifySubscribe_;
  //! Consumer admin reference.
  CosNotifyChannelAdmin::ConsumerAdmin_var ca_;
  //! Consumer admin id.
  CosNotifyChannelAdmin::AdminID id_;
};

inline
CosNotifyChannelAdmin::AdminID 
ConsumerAdmin::id() const {
  return id_;
}

inline
void 
ConsumerAdmin::setConsumerAdmin(CosNotifyChannelAdmin::ConsumerAdmin_ptr _ca) {
  ca_ = CosNotifyChannelAdmin::ConsumerAdmin::_duplicate(_ca);
}

#endif // notifyObserver_ConsumerAdmin_h
