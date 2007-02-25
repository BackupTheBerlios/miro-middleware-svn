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
#include "ConsumerAdmin.h"
#include "ProxySupplier.h"
#include "NotifySubscribe.h"

#include "miro/Log.h"

#include <qlistview.h>
#include <qpopupmenu.h>

#include <algorithm>

ConsumerAdmin::ConsumerAdmin(CosNotifyChannelAdmin::ConsumerAdmin_ptr _ca,
			     QListViewItem * _parentItem,
			     QObject * _parent, char const * _name) :
  Super(_parentItem, NULL, _parent, _name),
  notifySubscribe_(new NotifySubscribe(_ca, 
				       listViewItem(),
				       this, "notify_subscribe")),
  ca_(CosNotifyChannelAdmin::ConsumerAdmin::_duplicate(_ca)),
  id_(ca_->MyID())
{
  QString n;
  n.setNum(id_);
  setName(n);

  listViewItem()->setText(0, n);  listViewItem()->setText(0, name());
  listViewItem()->setText(1, className());

  CosNotifyChannelAdmin::ProxyIDSeq_var proxyIDs;

  proxyIDs = ca_->push_suppliers();
  for (unsigned int i = 0; i < proxyIDs->length(); ++i) {
    new ProxySupplier(ca_->get_proxy_supplier(proxyIDs[i]),
		      proxyIDs[i],
		      listViewItem(), this);
  }

#ifdef TAO_NOTIFY_HAS_PULL_CONSUMERS
  proxyIDs = ca_->pull_suppliers();
  for (unsigned int i = 0; i < proxyIDs->length(); ++i) {
    new ProxySupplier(ca_->get_proxy_suppliers(proxyIDs[i]),
		      proxyIDs[i],
		      listViewItem(), this);
  }
#endif
}

void
ConsumerAdmin::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Create supplier", this, SLOT(createProxyPushSupplier()));
  _menu.insertSeparator();
  notifySubscribe_->contextMenu(_menu);
  _menu.insertSeparator();
  _menu.insertItem("Destroy", this, SLOT(destroy()));
}

void
ConsumerAdmin::destroy()
{
  // destroy channel at the factory
  ca_->destroy();
}

void
ConsumerAdmin::createProxyPushSupplier()
{
  CosNotifyChannelAdmin::ProxyID id;

  ca_->obtain_notification_push_supplier(CosNotifyChannelAdmin::STRUCTURED_EVENT, id);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
ConsumerAdmin::getProxyPushSupplier(CosNotifyChannelAdmin::ProxyID _id)
{
  return ca_->get_proxy_supplier(_id);
}
