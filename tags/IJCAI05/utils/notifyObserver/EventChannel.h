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
