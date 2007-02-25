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
#include "ProxySupplier.h"

#include <qpopupmenu.h>
#include <qlistview.h>
#include <qdialog.h>
#include <qgroupbox.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qpushbutton.h>

ProxySupplier::ProxySupplier(CosNotifyChannelAdmin::ProxySupplier_ptr _ps,
			     CosNotifyChannelAdmin::ProxyID _id,
			     QListViewItem * _parentItem,
			     QObject * _parent, char const * _name) :
  Super(_parentItem, NULL, _parent, _name),
  ps_(CosNotifyChannelAdmin::ProxySupplier::_duplicate(_ps)),
  id_(_id)
{
  QString n;
  n.setNum(id_);
  setName(n);

  listViewItem()->setText(0, n);
  listViewItem()->setText(1, className());
}

void
ProxySupplier::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Offers", this, SLOT(obtainOfferedTypes()));
}

void
ProxySupplier::obtainOfferedTypes()
{
  CosNotification::EventTypeSeq_var offers =
    ps_->obtain_offered_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);

  QDialog dialog(NULL, "offers");
  dialog.setCaption("Available offers");

  QVBoxLayout * topBox = new QVBoxLayout(&dialog, 0, -1, "boxLayout");

  QVGroupBox * fileBox = new QVGroupBox(&dialog, "eventBox");
  QListView * list = new QListView(fileBox, "list");

  list->setSelectionMode(QListView::NoSelection);
  list->addColumn("Domain name");
  list->addColumn("Type name");

  for (unsigned int i = 0; i < offers->length(); ++i) {
    QListViewItem * item = new QListViewItem(list);
    item->setText(0, static_cast<char const * const>(offers[i].domain_name));
    item->setText(1, static_cast<char const * const>(offers[i].type_name));
  }

  topBox->addSpacing(10);
  topBox->addWidget(fileBox);
  fileBox->setTitle("Offered Types:");

  topBox->addSpacing(10);
  QHBoxLayout * dialogButtonsBox = new QHBoxLayout(topBox, -1, "hBoxLayout");
  QSpacerItem * dBSpace = new QSpacerItem(0, 0);
  QPushButton * okButton = new QPushButton("OK", &dialog);
  
  topBox->addSpacing(5);
  dialogButtonsBox->addItem(dBSpace);
  dialogButtonsBox->addWidget(okButton);
  dialogButtonsBox->addSpacing(5);
  
  okButton->setDefault(true);

  // connect the dialogs functionality  
  connect(okButton, SIGNAL(clicked()), &dialog, SLOT(accept()));

  dialog.exec();
}
