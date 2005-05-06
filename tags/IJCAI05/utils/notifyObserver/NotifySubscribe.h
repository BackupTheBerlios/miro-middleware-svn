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
#ifndef NotifySubscribe_h
#define NotifySubscribe_h

#include "utils/widgets/Item.h"

#include <orbsvcs/CosNotifyCommC.h>

class NotifySubscribe : public Item
{
  Q_OBJECT;

  typedef Item Super;

public:
  NotifySubscribe(CosNotifyComm::NotifySubscribe_ptr _notifySubscribe,
		  QListViewItem * _parentItem,
		  QObject * _parent = 0, const char * _name = 0);

  void contextMenu(QPopupMenu& _menu);

protected slots:
  void subscriptionChange();

protected:
  CosNotifyComm::NotifySubscribe_var notifySubscribe_;
};

#endif // NotifySubscribe_h
