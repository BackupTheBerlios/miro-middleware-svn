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
#ifndef notifyObserver_SupplierAdmin_h
#define notifyObserver_SupplierAdmin_h

#include "utils/widgets/Item.h"


#include <orbsvcs/CosNotifyChannelAdminS.h>

// forward declarations
class NotifyPublish;

class SupplierAdmin : public Item
{
  Q_OBJECT
  
  typedef Item Super;

public:
  SupplierAdmin(CosNotifyChannelAdmin::SupplierAdmin_ptr _sa,
	       QListViewItem * _parentItem,
	       QObject * _parent = 0, const char * _name = 0);


  virtual void contextMenu(QPopupMenu& _menu);

  void setSupplierAdmin(CosNotifyChannelAdmin::SupplierAdmin_ptr _sa);

  CosNotifyChannelAdmin::ProxyConsumer_ptr
  getProxyPushConsumer(CosNotifyChannelAdmin::ProxyID _id);
  CosNotifyChannelAdmin::AdminID id() const;

signals:
  void updateAdmin();

protected slots:
  void destroy();
  void createProxyPushConsumer();

protected:
  //! NotifyPublish interface.
  NotifyPublish * notifyPublish_;
  //! Supplier admin reference.
  CosNotifyChannelAdmin::SupplierAdmin_var sa_;
  //! Supplier admin id.
  CosNotifyChannelAdmin::AdminID id_;
};

inline
CosNotifyChannelAdmin::AdminID 
SupplierAdmin::id() const {
  return id_;
}

inline
void 
SupplierAdmin::setSupplierAdmin(CosNotifyChannelAdmin::SupplierAdmin_ptr _sa) {
  sa_ = CosNotifyChannelAdmin::SupplierAdmin::_duplicate(_sa);
}

#endif // notifyObserver_SupplierAdmin_h
