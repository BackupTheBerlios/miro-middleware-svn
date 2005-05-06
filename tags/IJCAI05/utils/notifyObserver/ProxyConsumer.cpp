// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware For Robots)
//
// (c) 2004
// Department of Neural Information Processing, University of Ulm, Germany
//
// $Id$
// 
//////////////////////////////////////////////////////////////////////////////

#include "ProxyConsumer.h"

#include <qpopupmenu.h>
#include <qlistview.h>
#include <qdialog.h>
#include <qgroupbox.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qpushbutton.h>

ProxyConsumer::ProxyConsumer(CosNotifyChannelAdmin::ProxyConsumer_ptr _pc,
			     CosNotifyChannelAdmin::ProxyID _id,
			     QListViewItem * _parentItem,
			     QObject * _parent, char const * _name) :
  Super(_parentItem, NULL, _parent, _name),
  pc_(CosNotifyChannelAdmin::ProxyConsumer::_duplicate(_pc)),
  id_(_id)
{
  QString n;
  n.setNum(id_);
  setName(n);

  listViewItem()->setText(0, n);
  listViewItem()->setText(1, className());
}

void
ProxyConsumer::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Subscriptions", this, SLOT(obtainSubscriptionTypes()));
}

void
ProxyConsumer::obtainSubscriptionTypes()
{
  CosNotification::EventTypeSeq_var subscriptions =
    pc_->obtain_subscription_types(CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);

  QDialog dialog(NULL, "subscriptions");
  dialog.setCaption("Available subscriptions");

  QVBoxLayout * topBox = new QVBoxLayout(&dialog, 0, -1, "boxLayout");

  QVGroupBox * fileBox = new QVGroupBox(&dialog, "eventBox");
  QListView * list = new QListView(fileBox, "list");

  list->setSelectionMode(QListView::NoSelection);
  list->addColumn("Domain name");
  list->addColumn("Type name");

  for (unsigned int i = 0; i < subscriptions->length(); ++i) {
    QListViewItem * item = new QListViewItem(list);
    item->setText(0, static_cast<char const * const>(subscriptions[i].domain_name));
    item->setText(1, static_cast<char const * const>(subscriptions[i].type_name));
  }

  topBox->addSpacing(10);
  topBox->addWidget(fileBox);
  fileBox->setTitle("Subscribed Types:");

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
