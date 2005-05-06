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

#include "QoSAdmin.h"

QoSAdmin::QoSAdmin(CosNotification::QoSAdmin_ptr _qosAdmin,
		   QListViewItem * _parentItem,
		   QObject * _parent, const char * _name) :
  Super(_parentItem, NULL, _parent, _name),
  qosAdmin_(CosNotification::QoSAdmin::_duplicate(_qosAdmin))
{}
