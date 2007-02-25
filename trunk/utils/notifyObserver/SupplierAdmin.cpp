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
#include "SupplierAdmin.h"
#include "ProxyConsumer.h"
#include "NotifyPublish.h"

#include "miro/Log.h"

#include <qlistview.h>
#include <qpopupmenu.h>

#include <algorithm>

SupplierAdmin::SupplierAdmin(CosNotifyChannelAdmin::SupplierAdmin_ptr _sa,
			     QListViewItem * _parentItem,
			     QObject * _parent, char const * _name) :
  Super(_parentItem, NULL, _parent, _name),
  notifyPublish_(new NotifyPublish(_sa, 
				   listViewItem(),
				   this, "notify_publish")),
  sa_(CosNotifyChannelAdmin::SupplierAdmin::_duplicate(_sa)),
  id_(sa_->MyID())
{
  QString n;
  n.setNum(id_);
  setName(n);

  listViewItem()->setText(0, n);
  listViewItem()->setText(1, className());
  
  CosNotifyChannelAdmin::ProxyIDSeq_var proxyIDs;

  proxyIDs = sa_->push_consumers();
  for (unsigned int i = 0; i < proxyIDs->length(); ++i) {
    new ProxyConsumer(sa_->get_proxy_consumer(proxyIDs[i]),
		      proxyIDs[i],
		      listViewItem(), this);
  }

#ifdef TAO_NOTIFY_HAS_PULL_CONSUMERS
  proxyIDs = sa_->pull_consumers();
  for (unsigned int i = 0; i < proxyIDs->length(); ++i) {
    new ProxyConsumer(sa_->get_proxy_consumer(proxyIDs[i]),
		      proxyIDs[i],
		      listViewItem(), this);
  }
#endif

  
}

void
SupplierAdmin::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Create supplier", this, SLOT(createProxyPushConsumer()));
  _menu.insertSeparator();
  notifyPublish_->contextMenu(_menu);
  _menu.insertSeparator();
  _menu.insertItem("Destroy", this, SLOT(destroy()));
}

void
SupplierAdmin::destroy()
{
  // destroy channel at the factory
  sa_->destroy();
}

void
SupplierAdmin::createProxyPushConsumer()
{
  CosNotifyChannelAdmin::ProxyID id;

  sa_->obtain_notification_push_consumer(CosNotifyChannelAdmin::STRUCTURED_EVENT, id);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
SupplierAdmin::getProxyPushConsumer(CosNotifyChannelAdmin::ProxyID _id)
{
  return sa_->get_proxy_consumer(_id);
}
