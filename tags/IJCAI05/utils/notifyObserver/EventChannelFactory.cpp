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

#include "EventChannelFactory.h"
#include "EventChannel.h"

#include "miro/Log.h"

#include <qlistview.h>
#include <qpopupmenu.h>

#include <algorithm>
#include <iostream>

EventChannelFactory::EventChannelFactory(CosNotifyChannelAdmin::EventChannelFactory_ptr _notifyFactory,
					 QListView * _listView,
					 QObject * _parent, char const * _name) :
  Super(_listView, NULL, _parent, _name),
  notifyFactory_(_notifyFactory)
{
  listViewItem()->setText(0, name());
  listViewItem()->setText(1, className());
  
  CosNotifyChannelAdmin::ChannelIDSeq_var channelIDs;
  channelIDs = notifyFactory_->get_all_channels();

  for (unsigned int i = 0; i < channelIDs->length(); ++i) {
    CosNotifyChannelAdmin::EventChannel_ptr ec =
      notifyFactory_->get_event_channel(channelIDs[i]);
    new EventChannel(ec, channelIDs[i], listViewItem(), this);
  }
}

void
EventChannelFactory::update()
{
  // Local types

  // Vector of admin ids.
  typedef std::vector<CosNotifyChannelAdmin::ChannelID> IdVector;
  // Mapping from admin ids to ConsumerAdmins.
  typedef std::map<CosNotifyChannelAdmin::ChannelID, EventChannel*> ChannelMap;

  // Vector of new ConsumerAdmin ids.
  // Get all current ConsumerAdmin ids and put them into the vector.
  // Sort vector for further processing.
  IdVector newIDs;
  CosNotifyChannelAdmin::ChannelIDSeq_var channelIDs =
    notifyFactory_->get_all_channels();
  for (CORBA::ULong i = 0; i < channelIDs->length(); ++i)
    newIDs.push_back(channelIDs[i]);
  std::sort(newIDs.begin(), newIDs.end());

  // Vector and map of known Channels.
  ChannelMap channelMap;
  IdVector oldIDs;

  // Fill vector and map.
  // Sort vector for further processing.
  QListViewItem * item = listViewItem()->firstChild();
  while (item) {
    ItemMap::const_iterator i = itemMap_.find(item);
    MIRO_ASSERT(i != itemMap().end());

    EventChannel * eventChannel = dynamic_cast<EventChannel *>(i->second);
    if (eventChannel != NULL) {
      oldIDs.push_back(eventChannel->id());
      channelMap.insert(std::make_pair(eventChannel->id(), eventChannel));
    }

    item = item->nextSibling();
  }
  std::sort(oldIDs.begin(), oldIDs.end());

  // Vectors of the added, the removed and the kept
  // ConsumerAdmin ids.
  IdVector addedIDs;
  IdVector removedIDs;
  IdVector keptIDs;

  // Compute those vectors by set operations.
  std::set_difference(newIDs.begin(), newIDs.end(), 
		      oldIDs.begin(), oldIDs.end(),
		      std::back_insert_iterator<IdVector>(addedIDs));
  std::set_difference(oldIDs.begin(), oldIDs.end(),
		      newIDs.begin(), newIDs.end(),
		      std::back_insert_iterator<IdVector>(removedIDs));
  std::set_intersection(oldIDs.begin(), oldIDs.end(),
			newIDs.begin(), newIDs.end(),
			std::back_insert_iterator<IdVector>(keptIDs));

  IdVector::const_iterator first, last;

  last = removedIDs.end();
  for (first = removedIDs.begin(); first != last; ++first) {
    delete channelMap[*first];
  }

  last = keptIDs.end();
  for (first = keptIDs.begin(); first != last; ++first) {
    channelMap[*first]->setEventChannel(notifyFactory_->get_event_channel(*first));
    channelMap[*first]->update();
  }
  
  last = addedIDs.end();
  for (first = addedIDs.begin(); first != last; ++first)  {
    CosNotifyChannelAdmin::EventChannel_ptr ec =
      notifyFactory_->get_event_channel(*first);
    new EventChannel(ec, *first, listViewItem(), this);
  }
}

void
EventChannelFactory::contextMenu(QPopupMenu& _menu)
{
  _menu.insertItem("Create channel", this, SLOT(createChannel()));
}

void
EventChannelFactory::updateFactory()
{
  update();
}

CosNotifyChannelAdmin::EventChannel_ptr 
EventChannelFactory::getChannel(CosNotifyChannelAdmin::ChannelID _id)
{
  return notifyFactory_->get_event_channel(_id);
}

void
EventChannelFactory::createChannel()
{
  CosNotifyChannelAdmin::ChannelID id;
  // Initial qos specified to the factory when creating the EC.
  CosNotification::QoSProperties initialQos;
  // Initial admin props specified to the factory when creating the EC.
  CosNotification::AdminProperties initialAdmin;
  CosNotifyChannelAdmin::EventChannel_ptr ec;

  // create channel at the factory
  ec = notifyFactory_->create_channel(initialQos, initialAdmin, id);
  update();
}
