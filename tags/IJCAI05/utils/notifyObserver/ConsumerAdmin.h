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
