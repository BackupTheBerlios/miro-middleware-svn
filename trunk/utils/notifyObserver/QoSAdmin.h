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
#ifndef QoSAdmin_h
#define QoSAdmin_h

#include "utils/widgets/Item.h"

#include <orbsvcs/CosNotificationC.h>

//! Frontend for the QoSAdmin interface.
class QoSAdmin : public Item
{
  Q_OBJECT;

  typedef Item Super;

public:
  QoSAdmin(CosNotification::QoSAdmin_ptr _qosAdmin,
	   QListViewItem * _parentItem,
	   QObject * _parent = NULL, const char * _name = NULL);
  
protected:
  CosNotification::QoSAdmin_var qosAdmin_;
};
#endif // QoSAdmin_h
