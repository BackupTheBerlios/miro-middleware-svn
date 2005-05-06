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
#ifndef notifyObserver_ProxySupplier_h
#define notifyObserver_ProxySupplier_h

#include "utils/widgets/Item.h"

#include <orbsvcs/CosNotifyChannelAdminC.h>

class ProxySupplier : public Item
{
  Q_OBJECT
  
  typedef Item Super;

public:
  ProxySupplier(CosNotifyChannelAdmin::ProxySupplier_ptr _ps,
		CosNotifyChannelAdmin::ProxyID _id,
		QListViewItem * _parentItem,
		QObject * _parent = 0, const char * _name = 0);

  virtual void contextMenu(QPopupMenu& _menu);

//   void createLocalSupplier();
//   CosNotifyChannelAdmin::ProxySupplier_ptr
//   getProxyPushSupplier(CosNotifyChannelAdmin::ProxyID _id);

protected slots:
  void obtainOfferedTypes();

protected:
  /** Channel factory. */
  CosNotifyChannelAdmin::ProxySupplier_var ps_;
  CosNotifyChannelAdmin::ProxyID id_;
};

#endif // notifyObserver_ProxySupplier_h
