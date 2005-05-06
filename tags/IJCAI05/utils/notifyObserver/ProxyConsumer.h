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
#ifndef notifyObserver_ProxyConsumer_h
#define notifyObserver_ProxyConsumer_h

#include "utils/widgets/Item.h"

#include <orbsvcs/CosNotifyChannelAdminS.h>

class ProxyConsumer : public Item
{
  Q_OBJECT
  
  typedef Item Super;

public:
  ProxyConsumer(CosNotifyChannelAdmin::ProxyConsumer_ptr _pc,
		CosNotifyChannelAdmin::ProxyID _id,
		QListViewItem * _parentItem,
		QObject * _parent = 0, const char * _name = 0);

  virtual void contextMenu(QPopupMenu& _menu);

//   void createLocalSupplier();
//   CosNotifyChannelAdmin::ProxyConsumer_ptr
//   getProxyPushConsumer(CosNotifyChannelAdmin::ProxyID _id);

signals:
  void updateProxy();

protected slots:
  void obtainSubscriptionTypes();

protected:
  //! Proxy consumer.
  CosNotifyChannelAdmin::ProxyConsumer_var pc_;
  //! Id.
  CosNotifyChannelAdmin::ProxyID id_;
};

#endif // notifyObserver_ProxyConsumer_h
