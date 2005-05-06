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
#ifndef notifyObserver_SupplierAdmin_h
#define notifyObserver_SupplierAdmin_h

#include "utils/widgets/Item.h"


#include <orbsvcs/CosNotifyChannelAdminS.h>

// forward declarations
class NotifyPublish;

class SupplierAdmin : public Item
{
  Q_OBJECT
  
  typedef Item Super;

public:
  SupplierAdmin(CosNotifyChannelAdmin::SupplierAdmin_ptr _sa,
	       QListViewItem * _parentItem,
	       QObject * _parent = 0, const char * _name = 0);


  virtual void contextMenu(QPopupMenu& _menu);

  void setSupplierAdmin(CosNotifyChannelAdmin::SupplierAdmin_ptr _sa);

  CosNotifyChannelAdmin::ProxyConsumer_ptr
  getProxyPushConsumer(CosNotifyChannelAdmin::ProxyID _id);
  CosNotifyChannelAdmin::AdminID id() const;

signals:
  void updateAdmin();

protected slots:
  void destroy();
  void createProxyPushConsumer();

protected:
  //! NotifyPublish interface.
  NotifyPublish * notifyPublish_;
  //! Supplier admin reference.
  CosNotifyChannelAdmin::SupplierAdmin_var sa_;
  //! Supplier admin id.
  CosNotifyChannelAdmin::AdminID id_;
};

inline
CosNotifyChannelAdmin::AdminID 
SupplierAdmin::id() const {
  return id_;
}

inline
void 
SupplierAdmin::setSupplierAdmin(CosNotifyChannelAdmin::SupplierAdmin_ptr _sa) {
  sa_ = CosNotifyChannelAdmin::SupplierAdmin::_duplicate(_sa);
}

#endif // notifyObserver_SupplierAdmin_h
