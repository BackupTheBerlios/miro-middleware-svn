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

#include "EventChannel.h"
#include "ConsumerAdmin.h"
#include "SupplierAdmin.h"

#include "miro/Log.h"

#include <qlistview.h>
#include <qpopupmenu.h>

#include <algorithm>
#include <iostream>

EventChannel::EventChannel(CosNotifyChannelAdmin::EventChannel_ptr _ec,
			   CosNotifyChannelAdmin::AdminID _id,
			   QListViewItem * _parentItem,
			   QObject * _parent, char const * _name) :
  Super(_parent, _name),
  ec_(CosNotifyChannelAdmin::EventChannel::_duplicate(_ec)),
  id_(_id)
{
  QString n;
  n.setNum(id_);
  setName(n);

  setListViewItem(new QListViewItem(_parentItem));
  listViewItem()->setText(0, name());
  listViewItem()->setText(1, className());

  CosNotifyChannelAdmin::AdminIDSeq_var consumerAdminIDs =
    ec_->get_all_consumeradmins();
  for (CORBA::ULong i = 0; i < consumerAdminIDs->length(); ++i) {
    CosNotifyChannelAdmin::ConsumerAdmin_ptr ca =
      ec_->get_consumeradmin(consumerAdminIDs[i]);
    new ConsumerAdmin(ca, listViewItem(), this);
  }

  CosNotifyChannelAdmin::AdminIDSeq_var supplierAdminIDs =
    ec_->get_all_supplieradmins();
  for (CORBA::ULong i = 0; i < supplierAdminIDs->length(); ++i) {
    CosNotifyChannelAdmin::SupplierAdmin_ptr ca =
      ec_->get_supplieradmin(supplierAdminIDs[i]);
    new SupplierAdmin(ca, listViewItem(), this);
  }
}

void
EventChannel::update()
{
  // Local types

  // Vector of admin ids.
  typedef std::vector<CosNotifyChannelAdmin::AdminID> IdVector;
  // Mapping from admin ids to ConsumerAdmins.
  typedef std::map<CosNotifyChannelAdmin::AdminID, ConsumerAdmin*> CAdminMap;
  // Mapping from admin ids to SupplierAdmins.
  typedef std::map<CosNotifyChannelAdmin::AdminID, SupplierAdmin*> SAdminMap;

  // Vector of new ConsumerAdmin ids.
  // Get all current ConsumerAdmin ids and put them into the vector.
  // Sort vector for further processing.
  IdVector newCIDs;
  CosNotifyChannelAdmin::AdminIDSeq_var consumerAdminIDs =
    ec_->get_all_consumeradmins();
  for (CORBA::ULong i = 0; i < consumerAdminIDs->length(); ++i)
    newCIDs.push_back(consumerAdminIDs[i]);
  std::sort(newCIDs.begin(), newCIDs.end());

  // Vector of new SupplierAdmin ids.
  // Get all current SupplierAdmin ids and put them into the vector.
  // Sort vector for further processing.
  IdVector newSIDs;
  CosNotifyChannelAdmin::AdminIDSeq_var supplierAdminIDs =
    ec_->get_all_supplieradmins();
  for (CORBA::ULong i = 0; i < supplierAdminIDs->length(); ++i)
    newSIDs.push_back(supplierAdminIDs[i]);
  std::sort(newSIDs.begin(), newSIDs.end());


  // Vector and map of known ConumserAdmins.
  CAdminMap cAdminMap;
  IdVector oldCIDs;
  // Vecotr and map of known SupplierAdmins.
  SAdminMap sAdminMap;
  IdVector oldSIDs;

  // Fill the vectors and maps.
  // Sort the vectors for further processing.
  QListViewItem * item = listViewItem()->firstChild();
  while (item) {
    ItemMap::const_iterator i = itemMap_.find(item);
    MIRO_ASSERT(i != itemMap().end());

    ConsumerAdmin * consumerAdmin = dynamic_cast<ConsumerAdmin *>(i->second);
    if (consumerAdmin != NULL) {
      oldCIDs.push_back(consumerAdmin->id());
      cAdminMap.insert(std::make_pair(consumerAdmin->id(), consumerAdmin));
    }

    SupplierAdmin * supplierAdmin = dynamic_cast<SupplierAdmin *>(i->second);
    if (supplierAdmin != NULL) {
      oldSIDs.push_back(supplierAdmin->id());
      sAdminMap.insert(std::make_pair(supplierAdmin->id(), supplierAdmin));
    }
    item = item->nextSibling();
  }
  std::sort(oldCIDs.begin(), oldCIDs.end());
  std::sort(oldSIDs.begin(), oldSIDs.end());


  // Vectors of the added, the removed and the kept
  // ConsumerAdmin ids.
  IdVector addedCIDs;
  IdVector removedCIDs;
  IdVector keptCIDs;

  // Compute those vectors by set operations.
  std::set_difference(newCIDs.begin(), newCIDs.end(), 
		      oldCIDs.begin(), oldCIDs.end(),
		      std::back_insert_iterator<IdVector>(addedCIDs));
  std::set_difference(oldCIDs.begin(), oldCIDs.end(),
		      newCIDs.begin(), newCIDs.end(),
		      std::back_insert_iterator<IdVector>(removedCIDs));
  std::set_intersection(oldCIDs.begin(), oldCIDs.end(),
			newCIDs.begin(), newCIDs.end(),
			std::back_insert_iterator<IdVector>(keptCIDs));

  IdVector::const_iterator first, last;

  last = removedCIDs.end();
  for (first = removedCIDs.begin(); first != last; ++first) {
    delete cAdminMap[*first];
  }

  last = keptCIDs.end();
  for (first = keptCIDs.begin(); first != last; ++first) {
    cAdminMap[*first]->setConsumerAdmin(ec_->get_consumeradmin(*first));
    cAdminMap[*first]->update();
  }
  

  last = addedCIDs.end();
  for (first = addedCIDs.begin(); first != last; ++first)  {
    CosNotifyChannelAdmin::ConsumerAdmin_ptr ca =
      ec_->get_consumeradmin(*first);
    new ConsumerAdmin(ca, listViewItem(), this);
  }


  IdVector addedSIDs;
  IdVector removedSIDs;
  IdVector keptSIDs;

  std::set_difference(newSIDs.begin(), newSIDs.end(), 
		      oldSIDs.begin(), oldSIDs.end(),
		      std::back_insert_iterator<IdVector>(addedSIDs));
  std::set_difference(oldSIDs.begin(), oldSIDs.end(),
		      newSIDs.begin(), newSIDs.end(),
		      std::back_insert_iterator<IdVector>(removedSIDs));
  std::set_intersection(oldSIDs.begin(), oldSIDs.end(),
			newSIDs.begin(), newSIDs.end(),
			std::back_insert_iterator<IdVector>(keptSIDs));

  last = removedSIDs.end();
  for (first = removedSIDs.begin(); first != last; ++first) {
    delete sAdminMap[*first];
  }

  last = keptSIDs.end();
  for (first = keptSIDs.begin(); first != last; ++first) {
    sAdminMap[*first]->setSupplierAdmin(ec_->get_supplieradmin(*first));
    sAdminMap[*first]->update();
  }

  last = addedSIDs.end();
  for (first = addedSIDs.begin(); first != last; ++first)  {
    CosNotifyChannelAdmin::SupplierAdmin_ptr sa =
      ec_->get_supplieradmin(*first);
    new SupplierAdmin(sa, listViewItem(), this);
  }
}

void
EventChannel::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Create consumer admin", this, SLOT(createConsumerAdmin()));
  _menu.insertItem("Create supplier admin", this, SLOT(createSupplierAdmin()));
  _menu.insertSeparator();
  _menu.insertItem("Destroy", this, SLOT(destroy()));
}

void
EventChannel::createConsumerAdmin()
{
  CosNotifyChannelAdmin::AdminID id;
  CosNotifyChannelAdmin::ConsumerAdmin_ptr ca =
    ec_->new_for_consumers(CosNotifyChannelAdmin::OR_OP, id);
  new ConsumerAdmin(ca, listViewItem(), this);
}

void
EventChannel::createSupplierAdmin()
{
  CosNotifyChannelAdmin::AdminID id;
  CosNotifyChannelAdmin::SupplierAdmin_ptr sa =
    ec_->new_for_suppliers(CosNotifyChannelAdmin::OR_OP, id);
  new SupplierAdmin(sa, listViewItem(), this);
}
void
EventChannel::destroy()
{
  // destroy channel at the factory
  ec_->destroy();
  delete this;
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr 
EventChannel::getConsumerAdmin(CosNotifyChannelAdmin::AdminID _id)
{
  return ec_->get_consumeradmin(_id);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr 
EventChannel::getSupplierAdmin(CosNotifyChannelAdmin::AdminID _id)
{
  return ec_->get_supplieradmin(_id);
}

