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
#ifndef NotifyPublish_h
#define NotifyPublish_h

#include "utils/widgets/Item.h"


#include <orbsvcs/CosNotifyCommC.h>

class NotifyPublish : public Item
{
  Q_OBJECT;

  typedef Item Super;

public:
  NotifyPublish(CosNotifyComm::NotifyPublish_ptr _notifyPublish,
		QListViewItem * _parentItem,
		QObject * _parent = 0, const char * _name = 0);

  void contextMenu(QPopupMenu& _menu);

protected slots:
  void offerChange();

protected:
  CosNotifyComm::NotifyPublish_var notifyPublish_;
};

#endif // NotifyPublish_h
